        /*  Unit test to check if terminate() works correctly, and if create_task( )
              (1) correctly initializes TCBs, and,
              (2) correctly updates the ReadyList
            assumes that you have used the name ReadyList to point to the Ready list
            and assumes that you have written all of your kernel functions in a single
            C file called kernel_functions.c
            And of course, init_kernel() and run() also need to work correctly
         */

 unsigned int g0=0, g1=0, g2=0, g3=1; 
/* gate flags for various stages of unit test */

 unsigned int low_deadline  = 1000;    
 unsigned int high_deadline = 100000;

mailbox *charMbox; 
mailbox *intMbox; 
mailbox *floatMbox;


void task_body_1(){
  char  varChar_t1;
  int   retVal_t1, varInt_t1;
  float varFloat_t1; 
  
  // test on an empty mailbox
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 != FAIL ) { g1 = FAIL; while(1) {}}
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 != FAIL ) { g1 = FAIL; while(1) {}}
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 != FAIL ) { g1 = FAIL; while(1) {}}

  // now send
  varChar_t1 = 'a';
  retVal_t1 = send_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 != OK ) 
  {    
      g1 = FAIL; 
      while(1) {}
  }
  varChar_t1 = 'b';
  retVal_t1 = send_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 != OK ) 
  {    
      g1 = FAIL; 
      while(1) {}
  }
  varChar_t1 = 'c';
  retVal_t1 = send_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 != OK ) 
  {    
      g1 = FAIL; 
      while(1) {}
  }

  // now read
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 == OK ) 
  { 
    if (varChar_t1 != 'a')
    { 
      g1 = FAIL; 
      while(1) {}
    }
  }
  else {while(1) {}}
  
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 == OK ) 
  { 
    if (varChar_t1 != 'b')
    { 
      g1 = FAIL; 
      while(1) {}
    }
  }
  else {while(1) {}}
  
  retVal_t1 = receive_no_wait(charMbox, &varChar_t1);
  if ( retVal_t1 == OK ) 
  { 
    if (varChar_t1 != 'c')
    { 
      g1 = FAIL; 
      while(1) {}
    }
  }
  else {while(1) {}}

  varInt_t1 = 100;
  retVal_t1 = send_wait( intMbox, &varInt_t1 );
  if ( retVal_t1 != OK )  {while(1) {}}
  
  g3 = FAIL;
  varFloat_t1 = 3.14159;
  retVal_t1 = send_wait( floatMbox, &varFloat_t1 );
  if ( retVal_t1 == OK )  {while(1) {}}
  else { g3 = OK; }
  
  terminate();  /* if communication calls in this task worked */
  
}

void task_body_2(){
  int                 retVal_t2;
  int                 varInt_t2; 

  retVal_t2 = receive_wait( intMbox, &varInt_t2 );
  if ( varInt_t2 != 100 )  {while(1) {}}

  retVal_t2 = receive_wait( intMbox, &varInt_t2 );
  if ( varInt_t2 != 500 )  {while(1) {}}
  
  while(1)
  {
    if ( g3 == OK) break;
  }
  
  while(1) 
    { 
        /* Alles Gut ! This unit test has been passed */
    }
}

void task_body_3(){
  int                 retVal_t3;
  int                 varInt_t3;

  varInt_t3 = 500;
  retVal_t3 = send_wait( intMbox, &varInt_t3 );
 
  terminate();   
}

void labTwoTest()
{
  
  g0 = OK;
  exception retVal = init_kernel(); 
  if ( retVal != OK ) { g0 = FAIL; while(1) { /* no use going further */  } }
  
  if ( ReadyList->pHead != ReadyList->pTail ) { g0 = FAIL ;}
  if ( WaitingList->pHead != WaitingList->pTail )    { g0 = FAIL ;}
  if (   TimerList->pHead !=   TimerList->pTail )    { g0 = FAIL ;}
    
  if ( g0 != OK ) { while(1) { /* no use going further */  } }
  
  charMbox = create_mailbox( 3 , sizeof(char) );
  if ( retVal == NULL )
  { 
    g1 = FAIL; while(1) {  }
  }
   
  intMbox = create_mailbox( 3 , sizeof(int) );
  if ( retVal == NULL )
  { 
    g1 = FAIL; while(1) {  }
  }

  floatMbox = create_mailbox( 3 , sizeof(float) );
  if ( retVal == NULL )
  { 
    g1 = FAIL; while(1) {  }
  }

  retVal = create_task( task_body_1 , low_deadline );
  if ( retVal !=  OK ) { while(1) { /* no use going further */  } }
  
  retVal = create_task( task_body_2 , 8*high_deadline );
  if ( retVal !=  OK ) { while(1) { /* no use going further */  } }

  retVal = create_task( task_body_3 , 2*low_deadline );
  if ( retVal !=  OK ) { while(1) { /* no use going further */  } }
  
  g3 =FAIL;
  run();
  
  while(1){ /* something is wrong with run */}
} 