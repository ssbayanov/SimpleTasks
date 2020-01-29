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
    bool _isCome;
    SimpleTask *next;
};

class SimpleTasks
{

public:
    SimpleTasks()
    {
        first = NULL;
        last = NULL;
    }

    void run()
    {
        SimpleTask *tmp = first;
        while (tmp)
        {
            bool removed = false;
            unsigned long time = millis();
            if (time > tmp->_nextCall)
            {
                tmp->_nextCall = tmp->_delay + tmp->_nextCall;
                tmp->_isCome = true;
                tmp->_counter++;
                if (tmp->_callback)
                {
                    tmp->_callback();
                    if (tmp->_count > 0)
                        if (tmp->_counter >= tmp->_count)
                        {
                            tmp = remove(tmp->_taskID);
                            removed = true;
                        }
                }
            }
            if (tmp && !removed)
                tmp = tmp->next;
        }
    }
    /**
     * Remove element from list and returned previos elemet
    **/
    SimpleTask *remove(int taskID)
    {
        SimpleTask *tmp = first;
        if(tmp)
            if (tmp->_taskID == taskID)
            { 
                if (tmp->next)
                {
                    first = tmp->next;
                    delete tmp;
                    return first;
                }
                else
                {
                    first = NULL;
                    last = NULL;
                    delete tmp;
                    return NULL;
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
                    return tmp->next;
                }
            }
        }
    }

    SimpleTask *add(int taskID, unsigned long delayTask, void (*callback)() = NULL, unsigned long count = 0)
    {
        if (getTask(taskID))
        {
            Serial.println("Task [" + taskID + "] already exist");
            return NULL;
        }

        SimpleTask *t = new SimpleTask;
        t->_taskID = taskID;
        t->_delay = delayTask;
        t->_callback = callback;
        t->_nextCall = millis() + delayTask;
        t->_isCome = false;
        t->_count = count;
        t->_counter = 0;
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
        return t;
    }

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

    bool isCome(int taskID)
    {
        SimpleTask *t = getTask(taskID);
        if (t)
        {
            if (t->_isCome)
            {
                t->_isCome = false;
                if (t->_count > 0)
                    if (t->_counter >= t->_count)
                    {
                        remove(t->_taskID);
                    }
                return true;
            }
        }
        else
        {
            Serial.println("NO TASK \"" + taskName + "\"");
        }

        return false;
    }

private:
    SimpleTask *first;
    SimpleTask *last;
};

SimpleTasks tasks;

#endif
//TASKS_H
