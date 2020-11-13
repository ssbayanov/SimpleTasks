#ifndef TASKS_H
#define TASKS_H

struct SimpleTask
{    
    unsigned long _delay;
    unsigned long _nextCall;
    unsigned long _count;
    unsigned long _counter;
    void (*_callback)();
    void (*_stopCallback)();
    uint8_t _taskID;
    bool _isOnce;
    bool _isCame;
    bool _isActive;
    SimpleTask *next;
};

class SimpleTasks
{

public:
    SimpleTasks()
    {
        first = NULL;
        last = NULL;
        count = 0;
    }

/**
 * Check tasks
 **/
    void run()
    {
        SimpleTask *task = first;
        while (task)
        {
            // If task is active
            if(task->_isActive)
            {
                // Get current time
                unsigned long time = millis();

                // If current time > time of next call
                if (time >= task->_nextCall)
                {
                    // Calc time of next call
                    task->_nextCall = task->_delay + task->_nextCall;
                    task->_isCame = true;                    

                    // If task have callback, call it
                    if (task->_callback != NULL)
                    {
                        task->_callback(); 
                        // If task have count increse counter and disabel if counter over count
                        // For task withot callback counter increse after check isCame
                        if (task->_count > 0)
                        {
                            task->_counter++;
                            if (task->_counter >= task->_count)
                            {
                                disable(task->_taskID);
                            }
                        }                       
                    }

                    
                }
            }
            // Get next task
            task = task->next;            
        }
    }

    /**
     * Set callback function when task is diabled by user or end of counts
     * stopCallback is function called by disabling
     **/
    void setStopCallback(uint8_t taskID, void (*stopCallback)())
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            t->_stopCallback = stopCallback;
        }
    }

    /**
     * Remove element from list
    **/
    void remove(uint8_t taskID)
    {
        SimpleTask *task = first;
        if(task)
        // If remove first task
            if (task->_taskID == taskID)
            { 
                if (task->next)
                {
                    first = task->next;
                    delete task;
                    return;
                }
                else
                {
                    first = NULL;
                    last = NULL;
                    delete task;
                    return;
                }
            }
        while (task)
        {
            if (task->next)
            {
                if (task->next->_taskID == taskID)
                {
                    SimpleTask *deleting = task->next;
                    task->next = task->next->next;
                    if (deleting == last)
                    {
                        last = task;
                    }
                    delete deleting;
                    return;
                }
            }
        }
    }

/**
 * Add task to task list.
 * count is count of repeats. After end task disabled.
 * Return ID of task
**/
    uint8_t add(unsigned long delayTask, void (*callback)() = NULL, unsigned long count = 0)
    {

        SimpleTask *task = new SimpleTask;
        task->_taskID = this->count++;
        task->_delay = delayTask;
        task->_callback = callback;
        task->_stopCallback = NULL;
        task->_nextCall = millis() + delayTask;
        task->_isCame = false;
        task->_count = count;
        task->_counter = 0;
        task->_isActive = true;
        task->next = NULL;

        if (first == NULL)
        {
            first = task;
        }
        else
        {
            last->next = task;
        }
        last = task;
        return task->_taskID;
    }


    bool isCame(uint8_t taskID)
    {
        SimpleTask *task = getTask(taskID);
        if (task)
        {
            if (task->_isCame)
            {
                task->_isCame = false;
                if (task->_count > 0 && task->_callback != NULL)
                {
                    task->_counter++;
                    if (task->_counter >= task->_count)
                    {
                        disable(task->_taskID);
                    }
                }
                return true;
            }
        }
        return false;
    }

    bool isDisabled(uint8_t taskID)
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            return !t->_isActive;
        }
        return true;
    }

    void disable(uint8_t taskID)
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            t->_isActive = false;
            if(t->_stopCallback != NULL)
            {
                t->_stopCallback();
            }
        }
    }

    void disableWOCallback(uint8_t taskID)
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            t->_isActive = false;
        }
    }

    bool isEnabled(uint8_t taskID)
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            return t->_isActive;
        }
        return false;
    }

    void enable(uint8_t taskID)
    {
        SimpleTask *task = getTask(taskID);
        if(task) {
            task->_isActive = true;
            task->_nextCall = millis() + task->_delay;
            task->_counter = 0;
        }
    }

private:
    SimpleTask *getTask(uint8_t taskID)
    {
        SimpleTask *task = first;
        while (task)
        {
            if (task->_taskID == taskID)
            {
                return task;
            }

            task = task->next;
        }
        return NULL;
    }

    SimpleTask *first;
    SimpleTask *last;
    int count;
};

//SimpleTasks tasks;

#endif
//TASKS_H
