// NumeriPrimi.cpp : Definisce il punto di ingresso dell'applicazione.
//

#include "pch.h"
#include "framework.h"
#include "NumeriPrimi.h"

#define MAX_LOADSTRING 100
#define MAX_PRIMI 100000

// Variabili globali:
HINSTANCE hInst;                                // istanza corrente
WCHAR szTitle[MAX_LOADSTRING];                  // Testo della barra del titolo
WCHAR szWindowClass[MAX_LOADSTRING];            // nome della classe della finestra principale
ULONG numPrimo[MAX_PRIMI];
INT quantiPrimi = 0;

// Dichiarazioni con prototipo di funzioni incluse in questo modulo di codice:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI Calcolaprimo(LPVOID lpParam);

ULARGE_INTEGER leggiOra()
{
    ULARGE_INTEGER valTempo;
    FILETIME oraAttuale;

    // legge l'ora attuale
    GetSystemTimeAsFileTime(&oraAttuale);
    // carica l'ora in un ULARGE_INTEGER per operazioni future
    valTempo.LowPart = oraAttuale.dwLowDateTime;
    valTempo.HighPart = oraAttuale.dwHighDateTime;

    return valTempo;
}

DWORD WINAPI Calcolaprimo(LPVOID a) {
    INT i1;

    for (quantiPrimi = 1; quantiPrimi < MAX_PRIMI; quantiPrimi++) {
        numPrimo[quantiPrimi] = numPrimo[quantiPrimi - 1] + 1;
        do {
            numPrimo[quantiPrimi] = numPrimo[quantiPrimi] + 1;
            for (i1 = 1; i1 < quantiPrimi; i1++) {
                if ((numPrimo[quantiPrimi] % numPrimo[i1]) == 0) break;
            }

        } while (i1 < quantiPrimi);
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    HANDLE ThreadUgo;
    DWORD   dwThreadIdArray;

    // TODO: Inserire qui il codice.
    numPrimo[0] = 1;
    // prova a creare un Thread
    ThreadUgo = CreateThread(NULL, 0, Calcolaprimo, NULL, 0, &dwThreadIdArray);
    // Inizializzare le stringhe globali
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NUMERIPRIMI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Eseguire l'inizializzazione dall'applicazione:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NUMERIPRIMI));

    MSG msg;

    // Ciclo di messaggi principale:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNZIONE: MyRegisterClass()
//
//  SCOPO: Registra la classe di finestre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NUMERIPRIMI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NUMERIPRIMI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNZIONE: InitInstance(HINSTANCE, int)
//
//   SCOPO: Salva l'handle di istanza e crea la finestra principale
//
//   COMMENTI:
//
//        In questa funzione l'handle di istanza viene salvato in una variabile globale e
//        viene creata e visualizzata la finestra principale del programma.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Archivia l'handle di istanza nella variabile globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNZIONE: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  SCOPO: Elabora i messaggi per la finestra principale.
//
//  WM_COMMAND  - elabora il menu dell'applicazione
//  WM_PAINT    - Disegna la finestra principale
//  WM_DESTROY  - inserisce un messaggio di uscita e restituisce un risultato
//
//

VOID DisegnaNumero(HWND hWnd, HDC hdc) {
    RECT rectFinestra;
    PCWSTR testo = L"Ciao";
    CHAR stringa1[40];
    WCHAR stringa2[40];
    SIZE_T a;
    
    GetClientRect(hWnd, &rectFinestra);
    for (int i = quantiPrimi; i > 0; i--) {
        _ultoa_s(i, stringa1, 10);
        mbstowcs_s(&a, stringa2, 30, stringa1, 30);
        testo = stringa2;
        rectFinestra.left = 0;
        DrawText(hdc, testo, -1, &rectFinestra, DT_SINGLELINE);
        _ultoa_s(numPrimo[i], stringa1, 10);
        mbstowcs_s(&a, stringa2, 30, stringa1, 30);
        testo = stringa2;
        rectFinestra.left = 200;
        DrawText(hdc, testo, -1, &rectFinestra, DT_SINGLELINE);
        rectFinestra.top += 20;
        if (rectFinestra.top > rectFinestra.bottom) break;
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analizzare le selezioni di menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Aggiungere qui il codice di disegno che usa HDC...
            DisegnaNumero(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Gestore di messaggi per la finestra Informazioni su.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
