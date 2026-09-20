#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int my_board_init(void)
{
    printk("Board Initialized\n");
    return 0;
}

/**
 * @brief This macro function is defined at compile time
 * @param init_fn pointer to the function to be executed before main()
 * @param level multiple chronological levels can be defined (e.g., EARLY, PRE_KERNEL_1, PRE_KERNEL_2, POST_KERNEL, APPLICATION, and SMP)
 * @param prio priority within that level
 *
 * SMP stands for symmetric multi-processing. However, this is used for the specific case where the function needs the other CPUs already running.
 */
SYS_INIT(my_board_init, APPLICATION, 0);