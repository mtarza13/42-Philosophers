<a name="readme-top"></a>
<div align="center">
  <!-- Logo -->
  <a href="https://github.com/mtarza13/42-Philosophers">
    <img src="https://raw.githubusercontent.com/dpetrosy/42-Philosophers/main/README_files/logo.png" alt="Logo" width="80" height="80">
  </a>

  <!-- Project Name -->
  <h1>🍝 Philosophers Project</h1>

  <!-- Short Description -->
  <p align="center">
    <b>42 School - Dining Philosophers Problem</b><br>
    A multithreading project exploring concurrent programming and synchronization
  </p>

  <!-- Badges -->
  <p>
    <img src="https://img.shields.io/badge/Score-125%20%2F%20100-success?style=for-the-badge" />
    <img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c" />
    <img src="https://img.shields.io/badge/Threads-Multithreading-orange?style=for-the-badge" />
    <img src="https://img.shields.io/badge/42-Project-000000?style=for-the-badge&logo=42" />
  </p>

  <h3>
    <a href="#-about-project">📜 About</a>
    <span> · </span>
    <a href="#-the-problem">🧠 Problem</a>
    <span> · </span>
    <a href="#-implementation">⚙️ Implementation</a>
    <span> · </span>
    <a href="#-usage">🚀 Usage</a>
    <span> · </span>
    <a href="#-testing">🧪 Testing</a>
  </h3>
</div>

---

## 📜 About Project

The **Philosophers** project is a classic computer science problem that explores the challenges of **concurrent programming** and **resource sharing**. This implementation uses **POSIX threads** (pthreads) and **mutexes** to solve the dining philosophers problem while avoiding **deadlocks** and **race conditions**.

### 🎯 Learning Objectives
- Master **multithreading** concepts
- Understand **mutex** synchronization
- Handle **shared memory** safely
- Prevent **deadlocks** and **race conditions**
- Work with **POSIX threads** (pthreads)

> [!NOTE]
> This project follows **42 School coding standards**:
> - Maximum 25 lines per function
> - Variables declared at function top
> - Only allowed functions may be used
> - No memory leaks or undefined behavior

---

## 🧠 The Problem

### 📖 Dining Philosophers Problem

The classic synchronization problem introduced by **Edsger Dijkstra**:

```
    🍝
   /   \
  🥄     🥄
 👨      👨
🥄        🥄
 👨  🥄  👨
   \   /
    👨
```

#### Rules:
- **N philosophers** sit around a circular table
- **N forks** placed between each philosopher  
- Each philosopher needs **2 forks** to eat
- Philosophers alternate between: **thinking** → **eating** → **sleeping**
- If a philosopher doesn't eat within `time_to_die`, they **die**
- Philosophers **cannot communicate** with each other

#### Challenge:
Coordinate access to shared resources (forks) without causing:
- **Deadlocks** (all philosophers stuck waiting)
- **Starvation** (a philosopher never gets to eat)
- **Race conditions** (data corruption)

---

## ⚙️ Implementation

### 🏗️ Architecture

```
📁 Project Structure
├── 📄 main.c              # Program entry point
├── 📄 parsing.c           # Argument validation
├── 📄 init.c              # Initialize simulation
├── 📄 init_philos.c       # Initialize philosophers
├── 📄 philosopher.c       # Philosopher behavior
├── 📄 forks.c             # Fork management
├── 📄 monitoring.c        # Death/completion monitoring
├── 📄 monitoring_utils.c  # Monitoring utilities
├── 📄 utils.c             # General utilities
├── 📄 utils2.c            # Additional utilities
├── 📄 philo.h             # Header file
└── 📄 Makefile            # Build configuration
```

### 🔧 Key Components

#### Data Structures
```c
/**
 * @brief Enumeration representing the different states a philosopher can be in
 * 
 * This enum defines all possible states during the simulation lifecycle.
 * Each state represents a specific action or condition of a philosopher.
 */
typedef enum e_philo_state
{
	EATING = 0,		/**< Philosopher is currently eating (has both forks) */
	SLEEPING = 1,	/**< Philosopher is sleeping after eating */
	THINKING = 2,	/**< Philosopher is thinking (waiting for forks or between actions) */
	DEAD = 3,		/**< Philosopher has died from starvation */
	FULL = 4,		/**< Philosopher has eaten the required number of meals */
	DEFU = 5		/**< Default/undefined state (initialization state) */
}					t_state;

/* Forward declaration to avoid circular dependency */
struct	s_data;

/**
 * @brief Structure representing an individual philosopher
 * 
 * Contains all data specific to each philosopher including their state,
 * meal tracking, timing information, and synchronization primitives.
 */
typedef struct s_philo
{
	/* Philosopher identification and meal tracking */
	int				id_philo;			/**< Unique identifier for the philosopher (1 to N) */
	int				meals_count;		/**< Number of meals this philosopher has eaten */
	long long		last_time_t_eat;	/**< Timestamp of when this philosopher last started eating */

	/* Reference to shared simulation data */
	struct s_data	*data;				/**< Pointer to shared simulation data structure */
	
	/* Current state of the philosopher */
	t_state			state;				/**< Current state (eating, sleeping, thinking, etc.) */

	/* Fork synchronization - each philosopher has access to two forks */
	pthread_mutex_t	*left_fork;			/**< Mutex for the fork on the philosopher's left */
	pthread_mutex_t	*right_fork;		/**< Mutex for the fork on the philosopher's right */
	
	/* Individual philosopher data protection mutexes */
	pthread_mutex_t	mut_state;			/**< Mutex to protect state changes */
	pthread_mutex_t	mut_meals_count;	/**< Mutex to protect meals_count modifications */
	pthread_mutex_t	mut_last_time_t_eat;/**< Mutex to protect last_time_t_eat modifications */

}					t_philo;

/**
 * @brief Main data structure containing all simulation parameters and shared resources
 * 
 * This structure holds all the global simulation data including timing parameters,
 * philosopher array, synchronization primitives, and monitoring threads.
 * It serves as the central hub for all shared information in the simulation.
 */
typedef struct s_data
{
	/* Timing parameters (in milliseconds) */
	int				time_t_eat;			/**< Time a philosopher spends eating */
	int				time_t_sleep;		/**< Time a philosopher spends sleeping */
	int				time_t_die;			/**< Maximum time without eating before death */
	long long		time_t_start;		/**< Timestamp when simulation started */
	
	/* Simulation control */
	bool			is_active;			/**< Flag indicating if simulation is still running */

	/* Simulation parameters */
	int				nb_philos;			/**< Total number of philosophers in simulation */
	int				nb_meals;			/**< Required number of meals per philosopher (-1 if unlimited) */

	/* Mutexes for protecting shared timing and control data */
	pthread_mutex_t	mut_time_t_eat;		/**< Mutex for time_t_eat access */
	pthread_mutex_t	mut_time_t_sleep;	/**< Mutex for time_t_sleep access */
	pthread_mutex_t	mut_time_t_die;		/**< Mutex for time_t_die access */
	pthread_mutex_t	mut_time_t_start;	/**< Mutex for time_t_start access */
	pthread_mutex_t	mut_print;			/**< Mutex for synchronized console output */
	pthread_mutex_t	mut_is_active;		/**< Mutex for is_active flag protection */
	pthread_mutex_t	mut_nb_philos;		/**< Mutex for nb_philos access */
	
	/* Fork mutexes array - shared resources between philosophers */
	pthread_mutex_t	*fork;				/**< Array of fork mutexes (nb_philos elements) */

	/* Philosopher array */
	t_philo			*philos;			/**< Array of all philosophers in the simulation */

	/* Monitoring threads for simulation control */
	pthread_t		monit_death;		/**< Thread monitoring for philosopher deaths */
	pthread_t		monit_dining_complete;	/**< Thread monitoring for dining completion */
	
	/* Philosopher threads array */
	pthread_t		*philosopher_th;	/**< Array of philosopher thread handles */

}					t_data;

```

#### Synchronization Strategy
1. **Fork Ordering**: Prevent deadlocks by ordering fork acquisition
2. **Mutex Protection**: Guard shared data access
3. **Atomic Operations**: Ensure thread-safe state changes
4. **Death Monitoring**: Separate monitoring thread checks for deaths

---

## 🚀 Usage

### Prerequisites
- **GCC compiler**
- **POSIX-compliant system** (Linux, macOS)
- **pthread library**

### Compilation
```bash
# Clone the repository
git clone https://github.com/mtarza13/42-Philosophers.git
cd 42-Philosophers

# Compile the project
make

# Clean object files
make clean

# Remove all generated files
make fclean

# Recompile everything
make re
```

### Execution
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

#### Parameters:
- **`number_of_philosophers`**: Number of philosophers (and forks) [1-200]
- **`time_to_die`**: Time in ms before a philosopher dies without eating
- **`time_to_eat`**: Time in ms a philosopher spends eating
- **`time_to_sleep`**: Time in ms a philosopher spends sleeping
- **`number_of_times_each_philosopher_must_eat`**: *(Optional)* Simulation stops when all philosophers have eaten this many times

#### Example Usage:
```bash
# 5 philosophers, 800ms to die, 200ms eating, 200ms sleeping
./philo 5 800 200 200

# With meal limit: stop after each philosopher eats 7 times
./philo 5 800 200 200 7

# Edge case: 1 philosopher (should die)
./philo 1 800 200 200

# Stress test: Many philosophers
./philo 100 410 200 200
```

---

## 🧪 Testing

### 📊 Test Scenarios

#### Basic Functionality
```bash
# Normal case - should run indefinitely without deaths
./philo 5 800 200 200

# With meal limit - should stop when all eat 7 times
./philo 4 410 200 200 7
```

#### Edge Cases
```bash
# Single philosopher - should die (can't eat with 1 fork)
./philo 1 800 200 200

# Tight timing - challenging synchronization
./philo 4 310 200 100

# Many philosophers - stress test
./philo 200 410 200 200
```

#### Expected Output Format
```
0 1 has taken a fork
0 1 is eating
0 3 has taken a fork
0 3 is eating
200 1 is sleeping
200 3 is sleeping
200 2 has taken a fork
200 4 has taken a fork
200 2 is eating
200 4 is eating
400 1 is thinking
400 3 is thinking
400 2 is sleeping
400 4 is sleeping
```

### ✅ Validation Checklist

- [ ] **No data races**: Use tools like `valgrind --tool=helgrind`
- [ ] **No memory leaks**: Check with `valgrind --leak-check=full`
- [ ] **No deadlocks**: Philosophers don't get stuck
- [ ] **Accurate timing**: Death detection within 10ms
- [ ] **Proper cleanup**: All threads and mutexes properly destroyed
- [ ] **Edge cases**: Handle 1 philosopher, invalid inputs
- [ ] **Performance**: Handles 200+ philosophers efficiently

### 🔍 Debugging Tools
```bash
# Check for data races
valgrind --tool=helgrind ./philo 5 800 200 200

# Check for memory leaks
valgrind --leak-check=full ./philo 5 800 200 200

# Monitor resource usage
top -p $(pgrep philo)
```

---

## 📸 Demo

![Philosophers Demo](Screenshot36.png)

---

## 🎓 Key Learnings

### Multithreading Concepts
- **Thread creation** and **synchronization**
- **Mutex** usage and **deadlock prevention**
- **Race condition** mitigation
- **Shared memory** management

### Problem-Solving Techniques
- **Resource ordering** to prevent circular wait
- **Time-based monitoring** for death detection
- **Atomic operations** for thread safety
- **Graceful shutdown** handling

### System Programming
- **POSIX threads** (pthreads) API
- **Precise timing** with `gettimeofday()`
- **Signal handling** and **process control**
- **Memory management** in multithreaded environment

---

## 🔧 Technical Details

### Deadlock Prevention Strategy
The implementation uses **resource ordering** to prevent deadlocks:
```c
// Always acquire lower-numbered fork first
if (philo->id % 2 == 0) {
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(philo->left_fork);
} else {
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);
}
```

### Timing Precision
- Uses `gettimeofday()` for **microsecond precision**
- Custom **`usleep()` replacement** for accurate delays
- **Death monitoring** runs every 1ms for quick detection

### Thread Safety
- All **shared data** protected by mutexes
- **Print operations** synchronized to prevent output corruption
- **Atomic state changes** ensure consistency

---

## 📚 Resources

- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Deadlock Prevention Strategies](https://www.geeksforgeeks.org/deadlock-prevention/)
- [42 School Projects](https://github.com/42School)

---

## 👨‍💻 Author

**mtarza** - [GitHub Profile](https://github.com/mtarza13)

*42 School Student | System Programming Enthusiast*

---

## 📄 License

This project is part of the 42 School curriculum. Please respect the academic integrity policies of your institution.

---

<div align="center">
  <p>
    <b>⭐ If this project helped you, please give it a star! ⭐</b>
  </p>
  
  <p>
    <a href="#readme-top">🔝 Back to top</a>
  </p>
</div>
