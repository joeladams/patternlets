// **************************************
// Copyright (c) 2016 Victor Alessandrini
// All rights reserved.
// **************************************
// WSPool.h
// A thread pool class implementing work stealing
// ------------------------------------------
#include <iostream>
#include <BkBarrier.h>
#include <Task.h>
#include <Timer.h>
#include <WSDeque.h>
#include <RandInt.h>
#include <pthread.h>

// *********************************************
// Declare the thread function passed to threads
// *********************************************
void *ThreadFunc(void *P);

// --------------------------------------------------------------
// The purpose of this utility is to allow threads in a pool to
// know, by a call to ActiveTasks(), if all of them are inactive.
// -------------------------------------------------------------- 
class JobMgr
   {
   private:
   ///////
   int task_count;
   pthread_mutex_t job_mutex;

   public:
   ///////
    JobMgr()
       {
       pthread_mutex_init(&job_mutex, NULL);
       task_count = 0; 
       }

    void RegisterTask()
       {
       pthread_mutex_lock(&job_mutex);
       task_count++;
       pthread_mutex_unlock(&job_mutex);
       }
    
    void UnregisterTask()
       {
       pthread_mutex_lock(&job_mutex);
       task_count--;
       pthread_mutex_unlock(&job_mutex);
       }

    // ---------------------------------------------------------
    // Remember: locking the mutex to read is needed in order to
    // make sure that one is reading the last written value.
    // ---------------------------------------------------------
    int ActiveTasks()
       {
       int tc;
       pthread_mutex_lock(&job_mutex);
       tc = task_count;
       pthread_mutex_unlock(&job_mutex);
       return tc; 
       }
    };
      

class WSPool
   {
   private:
   ////////
    Timer T;
    BkBarrier *BB;               
    RandInt   *RI;
    WSDeque   *ws_queue;             // array of work stealing queues
    JobMgr    JM;                    // job manager, tracks tasks

    int       nThreads;
    bool      shut;

    pthread_t        *threads;
    pthread_attr_t   attr;
    pthread_mutex_t  pMutex;

    public:
    //////
    
    // Constructors and destructors
    // ----------------------------
    WSPool(int nTh)
       {
       nThreads = nTh;
       BB = new BkBarrier(nThreads);
       RI = new RandInt(nThreads);
       ws_queue = new WSDeque[nThreads];
       shut = false;

       // allocate memory for pthread_t identifiers
       // ------------------------------------------
       threads = (pthread_t *)malloc(sizeof(pthread_t)*(nThreads)); 
       if(threads == NULL) errno_abort("Malloc threads"); 

       // Initialize the threads attribute. All threads in the 
       // pool share, of course, the same attributes. They are 
       // joinable.
       // -----------------------------------------------------
       pthread_attr_init( &attr);
       pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE);
       pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM);

       // initialize mutex
       // ----------------
       int status = pthread_mutex_init( &pMutex, NULL);
       if(status != 0) err_abort(status, "Mutex init"); 

       // Launch the worker threads. Ranks are in [0, nThreads-1]
       // -------------------------------------------------------
       for (int n=0; n<nThreads; n++) 
          {
          status = pthread_create(&threads[n], &attr,
			      ThreadFunc, (void *)this);
          if(status != 0) err_abort(status, "Thread create");
          }
       T.Wait(500);
       }

    ~WSPool()
       {
       delete BB;
       delete RI;
       delete [] ws_queue;
       }

   // ---------------------------------------------------
   // Job submissions, called by client threads. Task and
   // TaskGroup classes are the same ones as in NPool
   // ---------------------------------------------------

   // Submit a job with a unique root task:
   // ------------------------------------
   void SubmitJob(Task *T)
       {
       ws_queue[0].push(T);
       JM.RegisterTask();
       BB->ReleaseThreads();
       }

   // Submit a job with several root tasks, encapsulated in a
   // TaskGroup container. Tasks are spread over all the
   // ws dequeues.
   // -------------------------------------------------------
   void SubmitJob(TaskGroup *TG)
       {
       int n = 0;
       std::list<Task*>::iterator pos;
       for(pos=TG->LT.begin(); pos!=TG->LT.end(); pos++)
          {
          Task *T = *pos;
          int index = n%nThreads; 
          ws_queue[index].push(T);    
          JM.RegisterTask();
          n++;
          }
       BB->ReleaseThreads();
       }

   // Launch a child task. Called by worker threads
   // ---------------------------------------------
   void SubmitChild(Task *T)
       {
       int myrank = GetRank();
       if(myrank<0) std::cout << "\n rank error " << std::endl;
       ws_queue[myrank].push(T);
       JM.RegisterTask();
       }

   // Wait for job termination. Called by client threads
   // --------------------------------------------------
   void WaitForIdle()
      {
      BB->WaitForIdle();
      }

   // -----------------------------------------------
   // This is the thread function executed by all the
   // worker threads during their lifetime.
   // -----------------------------------------------
   void PeerThread()
      {
      bool my_shut;
      int rank = GetRank();

      for(;;)
         {
         BB->Wait();        // Here worker threads sleep

         pthread_mutex_lock(&pMutex);
         my_shut = shut;
         pthread_mutex_unlock(&pMutex);

         if(my_shut == false) RunWorkload(rank);
         else break;
         }
      }

   // ----------------------------------------------------
   // This is the function called by worker threads to
   // run a specific job. Worker threads keep trying to
   // deque tasks from their propietary queues and, if
   // propietary queues are empty, they try to steal from
   // another "victim" queue. 
   // The JobMgr JM keeps track of the number of waiting
   // tasks, and is used to stop the recurrent procedure 
   // when there are no more tasks to run
   // --------------------------------------------------
   void RunWorkload(int rk)
      {
      int myrank = rk;
      int victim;
      Task *t;
      //std::cout << "\n Thread " << myrank << " starts" << std::endl;

      // -------------------------------------------------  
      // Enter here an infinite loop where tasks are poped
      // from queues. This infinite loop should work until
      // complete termination is detected.
      // -------------------------------------------------
      while(JM.ActiveTasks())
         {
         t = ws_queue[myrank].try_pop();
         if(t != NULL)
            {
            t->ExecuteTask();
            JM.UnregisterTask();
            }
         else
            {
            // get a victim to steal from
            // --------------------------
            victim = (myrank+1)%nThreads;
            if(!ws_queue[victim].isEmpty())
               {
               t = ws_queue[victim].try_steal();
               if(t != NULL)
                  {
                  t->ExecuteTask();
                  JM.UnregisterTask();
                  }
               }
            }
         }
      // std::cout << "\n Thread " << myrank << " exiting; active tasks = " 
      //          << JM.ActiveTasks() << std::endl;
      }

   // -----------------------------------------------------------
   // Called by client threads, before destroying the queue. This
   // is called when the client program does not intend to submit
   // new jobs.
   // -----------------------------------------------------------
   void JoinThreads()
      {
      pthread_mutex_lock(&pMutex);
      shut = true;
      pthread_mutex_unlock(&pMutex);
      BB->WaitForIdle(); 
      BB->ReleaseThreads();

      // Join the terminating threads
      // ----------------------------
      for(int n=0; n<nThreads; n++)
         {
         int status = pthread_join(threads[n], NULL);
         if(status) err_abort(status, "Thread join");
         }
      std::cout << "\n Threads joined" << std::endl;
      }


   // --------------------------------------------------
   // This function will be called by tasks if they need
   // to know the rank of the running thread 
   // ---------------------------------------------------
   int GetRank()
      {
      pthread_t my_id;
      int n, my_rank, status;

      my_id = pthread_self();          // determine who  am
   
      pthread_mutex_lock(&pMutex);
      n = -1;
      do
         {
         n++;
         status = pthread_equal(my_id, threads[n]);
         } while(status==0 && n < nThreads); 
      pthread_mutex_unlock(&pMutex);
 
      if(status) my_rank=n;    // OK, return rank
      else my_rank = (-1);     // else, return error
      return my_rank;
      }

   // end of WSPool class
   };


// *********************************************
// This is the thread function passed to threads
// *********************************************
void *ThreadFunc(void *P)
   {
   WSPool *tp = (WSPool *)P;
   tp->PeerThread();  // call member function in pool
   }
