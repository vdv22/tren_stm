#include "test.h"


void testButton ()
{
	off_all();
	  if (in_one()==0)         //1
	  {
		  on_one();
	  }
	  else if (in_two()==0)   //2
	  {
		  on_two();
	  }
	  else if (in_three()==0)  //3
	  {
		  on_three();
	  }
	  else if (in_four()==0)  //4
	  {
		  on_four();
	  }
	  else if (in_five()==0)  //5
	  {
		  on_five();
	  }
	  else if (in_six()==0 && (COUNT_LED==10))  //6
	  {
		  on_six();
	  }
	  else if ( in_seven()==0 && (COUNT_LED==10)) //7
	  {
		  on_seven();
	  }
	  else if (in_eight()==0 && (COUNT_LED==10)) //8
	  {
		  on_eight();
	  }
	  else if (in_nine()==0 && (COUNT_LED==10)) //9
	  {
		  on_nine();
	  }
	  else if (in_ten()==0 && (COUNT_LED==10)) //10
	  {
		  on_ten();
	  }
}
