#ifndef TASKS_H
#define TASKS_H

struct SimpleTask
{    
    unsigned long _delay;
    unsigned long _nextCall;
    unsigned long _count;
    unsigned long _counter;
    void (*_callback)();
    int _taskID;
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
        SimpleTask *tmp = first;
        while (tmp)
        {
            // If task is active
            if(tmp->_isActive)
            {
                // Get current time
                unsigned long time = millis();

                // If current time > time of next call
                if (time > tmp->_nextCall)
                {
                    // Calc time of next call
                    tmp->_nextCall = tmp->_delay + tmp->_nextCall;
                    tmp->_isCame = true;                    

                    // If task have callback, call it
                    if (tmp->_callback)
                    {
                        tmp->_callback(); 
                        // If task have count increse counter and disabel if counter over count
                        // For task withot callback counter increse after check isCame
                        if (tmp->_count > 0)
                        {
                            tmp->_counter++;
                            if (tmp->_counter >= tmp->_count)
                            {
                                disable(tmp->_taskID);
                            }
                        }                       
                    }

                    
                }
            }
            tmp = tmp->next;            
        }
    }

    /**
     * Remove element from list and returned previos elemet
    **/
    void remove(int taskID)
    {
        SimpleTask *tmp = first;
        if(tmp)
        // If remove first task
            if (tmp->_taskID == taskID)
            { 
                if (tmp->next)
                {
                    first = tmp->next;
                    delete tmp;
                    return;
                }
                else
                {
                    first = NULL;
                    last = NULL;
                    delete tmp;
                    return;
                }
            }
        while (tmp)
        {
            if (tmp->next)
            {
                if (tmp->next->_taskID == taskID)
                {
                    SimpleTask *deleting = tmp->next;
                    tmp->next = tmp->next->next;
                    if (deleting == last)
                    {
                        last = tmp;
                    }
                    delete deleting;
                    return;
                }
            }
        }
    }

/**
 * Add task
 * Return ID of task
**/
    int add(unsigned long delayTask, void (*callback)() = NULL, unsigned long count = 0)
    {

        SimpleTask *t = new SimpleTask;
        t->_taskID = count++;
        t->_delay = delayTask;
        t->_callback = callback;
        t->_nextCall = millis() + delayTask;
        t->_isCame = false;
        t->_count = count;
        t->_counter = 0;
        t->_isActive = true;
        t->next = NULL;

        if (first == NULL)
        {
            first = t;
        }
        else
        {
            last->next = t;
        }
        last = t;
        return t->_taskID;
    }


    bool isCame(int taskID)
    {
        SimpleTask *t = getTask(taskID);
        if (t)
        {
            if (t->_isCame)
            {
                t->_isCame = false;
                if (t->_count > 0 && !t->_callback)
                    if (t->_counter >= t->_count)
                    {
                        disable(t->_taskID);
                    }
                return true;
            }
        }
        return false;
    }

    void disable(int taskID)
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            t->_isActive = false;
        }
    }

    void enable(int taskID)
    {
        SimpleTask *t = getTask(taskID);
        if(t) {
            t->_isActive = true;
            t->_counter = 0;
        }
    }

private:
    SimpleTask *getTask(int taskID)
    {
        SimpleTask *tmp = first;
        while (tmp)
        {
            if (tmp->_taskID == taskID)
            {
                return tmp;
            }

            tmp = tmp->next;
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
