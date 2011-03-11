void boilerplate()
{
//  #define EXPDATE  "-8890"
//  O("This test copy expires ");
//  cd(show(X("_dj " EXPDATE)));
//  K x=_(1);
//  K y=X(EXPDATE  " < _ _T");
//  if(matchI(x,y))exit((I)show(kerr("exp")));
//  cd(x); cd(y);
  if(!isatty(fileno(stdout))) R;
  O("K Console - Enter \\ for help\n");
  prompt(0);

}

//Q. What if a script is \loaded (via remote call) while the terminal is waiting with an open PDA for matching parentheses/quote marks? 
//A. In K3.2 the call blocks until the command-line input is not in an intermediate state
//Q. What if remote calls are sent to the console while the console is still processing the scripts (while[1;`0:"busy "]) given as command line arguments?
//A. In K3.2 the call blocks until the processing finishes or is interrupted
//Q. What if K is receiving a large message from a client. Does it block?
//A. ?
//Q. What if K is sending a large message to a client or server. Does it block?
//A. ?

K load(S s) //TODO: working dir is stable ... store then reset after reading scripts //TODO: 'file' loads 'file.k'
{
  FILE*f=fopen(s,"r");
  P(!f,_n())
  lines(f);
  fclose(f);
  R _n();
}

K backslash(S s, I n)
{
  S t;
  if(1==n) //TODO: if error or waiting on matching )]} then \ will resolve
    O("Backslash Commands:\n"
      "\\+      verb help\n"
      "\\'      adverb help\n"
      "\\:      I/O verb help\n"
      "\\[cmd]  system command (also \\[ cmd]), \\echo hello\n"
     "\\\\      exit (or ctrl+d)\n"
    
    ); 

  else if(!s[2] || isspace(s[2]))
  {
    t=s+(s[2]?3:2);
    SW(s[1])
    {
      CS('\\',exit(0))
      CS('\'', O(
               "Adverbs / \\ ' /: \\: ':\n"
               "Use adverbs in conjunction with verbs/functions\n"
               "Over and scan behave differently depending on whether their left argument\n"
               "is monadic or dyadic.\n"
               "/  over dyad    +/1 2 3  or 4+/1 2 3    */1+!5\n"
              "\\  scan dyad    +\\3 4 5  or 6+\\3 4 5    +\\!10\n"
               "/  over monad   apply until fixed    f/x    (%%[;2.0])/9999.0 \n"
               "/  over monad   apply n times      n f/x    4 (2+)/0 \n"
               "/  over monad   apply while true   b f/x    todo\n"
              "\\  scan monad   trace until repeat   f\\x    (1!)\\1 2 3 4\n"
              "\\  scan monad   trace n times      n f\\x    10(|+\\)\\1 1\n"
               "/  over         todo. f/[x;y;z]\n"
              "\\  scan         todo. f\\[x;y;z]\n"
               "'  each         \"abc\" ,' \"def\"    join each  \n"
               "'  each         !:' 2 3 4    enumerate each  \n"
               "/: eachright    #:/:(2;2 2;2 2 2)    count each right\n"
               "/: eachright    2 #/: (\"alpha\";\"bravo\";\"charlie\")    take each right\n"
              "\\: eachleft     0 1 2 3 #\\: \"abc\"     take each left\n"
               "': eachpair     apply pairwise  -':1 3 4 8 10\n"
               );
      )
      CS('+', O(
              "Verbs +-*%%|&^!<>=~@?_,#$.:\n"
              "Verbs work on all sensible types (ints, floats, strings, symbols, lists)\n"
              "Dyadic or monadic is determined from context, default is dyadic\n"
              "Add : after a verb to force the monadic form, + is plus, +: is flip\n"
              "+ monadic  flip. transpose a matrix (a depth-2 list)\n"
              "+ dyadic   plus. add numbers together\n"
              "- monadic  negate. invert sign\n"
              "- dyadic   minus. subtraction\n"
              "* monadic  first. first element from the list\n"
              "* dyadic   times. multiply two numbers\n"
              "%% monadic  reciprocal. 1 over x \n"
              "%% dyadic   divide. x divided by y (not mod) \n"
              "| monadic  reverse. reverse order of list\n"
              "| dyadic   max/or. MAX(x,y) or OR(x,y) \n"
              "& monadic  where. &0 0 1 0 3 yields 2 4 4 4. \n"
              "& dyadic   min/and. MIN(x,y) or AND(x,y) \n"
              "^ monadic  shape. #elts at each depth (min over) \n"
              "^ dyadic   power. x to the exponent y \n"
              "! monadic  enumerate. !4 yields 0 1 2 3\n"
              "! dyadic   mod/rotate. 5!3 yields 2;  1 ! 4 5 6 yields 5 6 4 \n"
              "< monadic  grade up. indices of list sorted ascending \n"
              "< dyadic   less. boolean is x less than y\n"
              "> monadic  grade down. indices of list sorted descending\n"
              "> dyadic   more. boolean is x greater than y\n"
              "= monadic  group. =3 4 3 4 4 yields (0 2;1 3 4)  \n"
              "= dyadic   equals. 1 2 3 = 1 2 4 yields 1 1 0 (tolerantly) \n"
              "~ monadic  not. ~ 0 1 0 2 0 yields 1 0 1 0 1 \n"
              "~ dyadic   match. 1 2 ~ 1 2 yields 1 (types must match)\n"
              "@ monadic  atom. boolean is arg an atom (as opposed to a list) \n"
              "@ dyadic   at. elts from x at indices y\n"
              "@ triadic  monadic amend. see \\. \n"
              "@ tetradic dyadic amend. see \\.\n"
              "? monadic  unique. distinct elts from a list \n"
              "? dyadic   find/invert. index of y in x  / {x^2} ? 2 yields sqrt(2) \n"
              "? triadic  invert-guess. secant method with clues ?[{2^x};17;4]\n"
              "_ monadic  floor. tolerant floor function \n"
              "_ dyadic   drop/cut. lose x elts from list y / separate into pieces \n"
              ", monadic  enlist. put x inside a 1-element list \n"
              ", dyadic   join. \"ab\",\"cd\" yields \"abcd\"\n"
              "# monadic  count. number of elements in list \n"
              "# dyadic   take/reshape. fill x elts from y \n"
              "$ monadic  format. cast to string \n"
              "$ dyadic   form/format. cast \n"
              ". monadic  execute/make dictionary. .\"1+1\" yields 2 \n"
              ". dyadic   value. 1 2 3 . ,2 yields 3. see \\. \n"
              ". triadic  monadic amend. see \\. \n"
              ". tetradic dyadic amend. see \\. \n"
              ":          overloaded with many operations. \n"
              );
      )
      CS('.',R NYI)
      CS('_',R NYI)
      CS('0',R NYI)
      CS(':',
             O("I/O Verbs 0: 1: 2: 3: 4: 5: 6:\n"
               "\nDisk\n"
               "0: dyadic   write text file `f 0: \"hello\" \n"
               "0: monadic  read text file  0: `f\n"
               "1: dyadic   write binary file `f 1: 4 5 6 \n"
               "1: monadic  read binary file  1: `f (mmapped)\n"
               "2: monadic  read binary file  2: `f (copied to memory)\n"
               "5: dyadic   append to binary file `f 5: ,7\n"
               "6: dyadic   write raw byte string `f 6: \"\\01\\02\\03\"\n"
               "6: monadic  read raw byte string  6: `f\n"
               "\nNetwork\n"
               "Start k listening for IPC on port 1234  ./k -i 1234\n" 
               "3: monadic  open handle    h: 3:(`\"192.168.1.101\";1234) \n"
               "3: monadic  close handle   3: h \n"
               "3: dyadic   asynchronous send, returns _n      h 3: \"a:2\"\n"
               "4: dyadic   synchronous send, returns result   h 4: \"1+1\"\n"

               "\nOther\n"
               "0: dyadic   write to console `0: \"hello\\n\" \n"
               "2: dyadic   dynamically load C function  a:`libfile 2:(`func,3); a[1;2;3]\n"
               "4: monadic  type of data [-4,7],  4: \"c\" /returns 3\n"
               "5: monadic  todo. printed representation. 5:1 2 3 /returns \"1 2 3\"\n"

               "\n0: and 1: both have versions for reading fields\n"
               "In all cases `f can instead be (`f;start;length)\n"
               "Reading fixed-width fields: \"cbsijfd IFCSDTZMm\" (\" \" is ignore)\n"
               "(type;width)0:`f    (\"IFC\";3 5 4)0:`f  /read rows like \"20 30.1 golf\\n\" \n"
               "(type;width)1:`f \n" 
               "\"c\" 1:`f for c in \"cid\", read bytes/ints/doubles\n"
               "Reading delimited text fields:\n"
               "(type;[,]delim)0:`f    todo\n"
               "c 1-byte char, b 1-byte int, s 2-byte int, i 4-byte int, f 4-byte float,\n"
               "d 8-byte double, \" \" 1-byte skip, I int, F float, C string, S string (sym), DTZMm Y? todo\n"
             );
      )
      CS('`',R NYI)
      CS('a',R NYI)
      CS('b',R NYI)
      CS('c',R NYI)
      CS('d',R NYI)
      CS('e',R NYI)
      CS('i',R NYI)
      CS('l',R load(t)) 
      CS('m',R NYI) //shows nonstandard system commands
      CS('p',R NYI)
      CS('r',R NYI) //see seedPRNG()
      CS('s',R NYI)
      CS('t',R backslash_t(t)) //TODO: also \t [digits]
      CS('v',R NYI)
      CS('w',R workspace(s)) //used,allocated,mapped. lfop: Linux & 'ps' use /proc/self/stat or /proc/<MY_PID>/stat
    }
    R _n();
  }

  // \kr \cd  ?

  if(isspace(s[1]))s++; //Allow system commands to be executed without preceding space
  R system(s)?DOE:_n();
}


K backslash_t(S s)
{
  I d=clock(); 
  cd(X(s));
  d=(clock()-d)/(CLOCKS_PER_SEC/1000);
  R Kf(d);
}


#ifdef __MACH__
#include <mach/mach_types.h> 
#endif
K workspace(S s)
{
  K z=newK(-1,3); U(z)

  #ifdef __MACH__
  struct task_basic_info t_info;
  mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
  if(KERN_SUCCESS != task_info(mach_task_self(),TASK_BASIC_INFO,&t_info,&t_info_count)) R 0;
  kI(z)[0]=t_info.resident_size;
  kI(z)[1]=t_info.virtual_size;
  kI(z)[2]=0;
  #endif

  R z;
}
