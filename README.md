# philosophers

---

![ALT TEXT](images/demo.gif "Demonstration of the program")

## Description

One or more philosophers sit at around a circular table with one bowls of spaghetti. Forks are placed between each pair of adjacent philosophers (There are as many forks as there are philosophers).

Each philosopher must alternately eat, think, or sleep. However, a philosopher can only eat spaghetti when they have both left and right forks. Each fork can be held by only one philosopher and so a philosopher can use the fork only if it is not being used by another philosopher. After an individual philosopher finishes eating, they need to put down both forks so that the forks become available to others. A philosopher can take the fork on their right or the one on their left as they become available, but cannot start eating before getting both forks.

Eating is not limited by the remaining amounts of spaghetti or stomach space; an infinite supply and an infinite demand are assumed.

The problem is how to design a discipline of behavior (a concurrent algorithm) such that no philosopher will starve; i.e., each can forever continue to alternate between eat, think, and sleep, assuming that no philosopher can know when others may want to eat, think, or sleep.

For more details on the problem, read [this Wikipedia article](https://en.wikipedia.org/wiki/Dining_philosophers_problem).

### Project

+ The "philo" folder uses a mutex for each fork
+ The "philo_bonus" folder uses a semaphore

## Usage
### Installation
> ```commandline
> git clone https://github.com/Kllaster/philosophers.git\
> && cd philosophers\
> && cd philo && make\
> && cd ../philo_bonus && make\
> && cd ../
>  ```

### Starting the program
```commandline
./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

+ **number_of_philosophers**: The number of philosophers and also the number of forks.
+ **time_to_die** (in milliseconds): If a philosopher didn’t start eating time_to_die milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
+ **time_to_eat** (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
+ **time_to_sleep** (in milliseconds): The time a philosopher will spend sleeping.

optional arguments:
+ **number_of_times_each_philosopher_must_eat**: If all philosophers have eaten at least number_of_times_each_philosopher_must_eat times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

### Example

---
The philosopher should not eat and should die!
```commandline
./philo 1 800 200 200
```
---
No one should die!
```commandline
./philo 5 800 200 200
```
---
No one should die and the simulation should stop when all the philosophers has 
eaten at least 7 times each.
```commandline
./philo 5 800 200 200 7
```
---
No one should die!
```commandline
./philo 4 410 200 200
```
---
A philosophers should die!
```commandline
./philo 4 310 200 200
```
---