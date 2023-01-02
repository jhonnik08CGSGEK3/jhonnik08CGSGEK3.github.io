/* Kiselev Evgenii, 09-3, 02.01.2023, EK3 */

#include <windows.h>
void main( void )
{
  char buf[100];
  int guess, min = 0, max = 100, run = 1;

  sprintf(buf, "Number pool: %i-%i", min, max);
  MessageBox(NULL, buf, "EK3_CGSG_T40GUESS", MB_OK | MB_ICONWARNING);

  while (run)
  {  
    guess = (min + max) / 2;
    sprintf(buf, "Is your number greater than %i?", guess);

    switch (MessageBox(NULL, buf, "EK3_CGSG_T40GUESS",
                       MB_YESNOCANCEL | MB_ICONbuf | MB_DEFBUTTON1))
    {
      case IDNO:
        max = guess;
        break;
      case IDYES:
        min = guess;
        break;
      case IDCANCEL:
        run = 0;
        break;
    }
  }

  sprintf(buf, "The guessed number is %i!", guess);
  MessageBox(NULL, buf, "EK3_CGSG_T40GUESS", MB_OK | MB_ICONINFORMATION);
}