/* FILE NAME   : main.c
 * PROGRAMMER  : EK3
 * LAST UPDATE : 18.01.2023
 * PURPOSE     : Window creation and usage.
 */

#include "units/units.h"

#define WND_CLASS_NAME "EK3 Animation project window class"

/* Link to processing function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS: 
 *  - HINSTANCE hInstance     - instance handle
 *  - HINSTANCE hPrevInstance - previous instance handle (not used)
 *  - CHAR *CmdLine           - command line
 *  - INT ShowCmd             - show command line flag
 * RETURNS:
 *   (INT) what is the result.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                    CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  SetDbgMemHooks();

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;  /* Background color - chosen in system */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* Cursor load (system) */
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);     /* Pictogram load (system) */
  wc.lpszMenuName = NULL;                   /* Resource menu name */
  wc.hInstance = hInstance;                 /* Handle of instance that registers classes */
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;        /* Class name */

  /* Class registration in system */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd = 
    CreateWindow(WND_CLASS_NAME,  /* Window class name */
    "The Globe",                  /* Window header */
    WS_OVERLAPPEDWINDOW,          /* Window style */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Window position (x, y) - by default */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Window size (w, h) - by default */
    NULL,                         /* Mother window handle */
    NULL,                         /* Loaded menu handle */
    hInstance,                    /* Instance handle */
    NULL);                        /* Pointer to additional parameters */

  EK3_AnimInit(hWnd);
  EK3_AnimUnitAdd(EK3_UnitCtrlCreate());
  //EK3_AnimUnitAdd(EK3_UnitMarkerCreate());
  EK3_AnimUnitAdd(EK3_UnitObjectCreate());
  EK3_AnimUnitAdd(EK3_UnitGridCreate());

  /* Show and redraw window */
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  /* Message processing loop, until message 'WM_QUIT' received */
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
    {
      EK3_AnimRender();
      EK3_AnimCopyFrame();
    }
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Window message processing function.
 * ARGUMENTS: 
 *  - HWND hWnd     - window handle
 *  - UINT Msg      - message number
 *  - WPARAM wParam - message parameter ('word parameter')
 *  - LPARAM lParam - message parameter ('long parameter')
 * RETURNS:
 *   (LRESULT) - depending on the message.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 47, 2, NULL);
    return 0;
  case WM_SIZE:
    EK3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == 'p')
      EK3_Anim.IsPause = !EK3_Anim.IsPause;
    else if (wParam == 27)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
    else if (wParam == 122)
      EK3_AnimFlipFullScreen(hWnd);
    return 0;
  case WM_MOUSEWHEEL:
    EK3_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_TIMER:
    EK3_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EK3_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    EK3_AnimClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 't46globe.c' FILE */