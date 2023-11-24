#include <Windows.h>
#include "resource.h"
#include <iostream>
#include <fstream>
#include <string>

struct Usuario {
    std::wstring nombre;
    std::wstring contrasena;

    Usuario(const std::wstring& _nombre, const std::wstring& _contrasena)
        : nombre(_nombre), contrasena(_contrasena) {}
};
struct Doctor : public Usuario {
    std::wstring doctor;
    std::wstring cedula;

    Doctor(const std::wstring& _nombre, const std::wstring& _contrasena, const std::wstring& _doctor, const std::wstring& _cedula)
        : Usuario(_nombre, _contrasena), doctor(_doctor), cedula(_cedula) {}
};
class ListaUsuarios {
public:
    ListaUsuarios() : primero(nullptr), ultimo(nullptr) {
        cargarDesdeArchivo(); // Cargar usuarios desde el archivo al iniciar la lista
    }

    ~ListaUsuarios() {
        guardarEnArchivo(); // Guardar usuarios en el archivo al destruir la lista
        // Liberar memoria al destruir la lista
        NodoUsuario* actual = primero;
        while (actual != nullptr) {
            NodoUsuario* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }

    void agregarUsuario(const std::wstring& nombre, const std::wstring& contrasena) {
        // Agregar un nuevo usuario al final de la lista
        Usuario nuevoUsuario(nombre, contrasena);
        NodoUsuario* nuevoNodo = new NodoUsuario(nuevoUsuario);

        if (ultimo == nullptr) {
            primero = ultimo = nuevoNodo;
        }
        else {
            nuevoNodo->anterior = ultimo;
            ultimo->siguiente = nuevoNodo;
            ultimo = nuevoNodo;
        }

        guardarEnArchivo(); // Guardar la lista actualizada en el archivo
    }

    bool verificarUsuario(const std::wstring& nombre, const std::wstring& contrasena) const {
        // Verificar si el usuario y la contraseña coinciden con algún usuario en la lista
        NodoUsuario* actual = primero;
        while (actual != nullptr) {
            if (actual->usuario.nombre == nombre && actual->usuario.contrasena == contrasena) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

private:
    struct NodoUsuario {
        Usuario usuario;
        NodoUsuario* anterior;
        NodoUsuario* siguiente;

        NodoUsuario(const Usuario& _usuario)
            : usuario(_usuario), anterior(nullptr), siguiente(nullptr) {}
    };

    NodoUsuario* primero;
    NodoUsuario* ultimo;

    // Funciones para cargar y guardar usuarios desde y hacia el archivo
    void cargarDesdeArchivo() {
        std::wifstream archivo("usuarios.dat", std::ios::binary);
        if (archivo.is_open()) {
            std::wstring nombre, contrasena;
            while (archivo >> nombre >> contrasena) {
                agregarUsuario(nombre, contrasena);
            }
            archivo.close();
        }
    }

    void guardarEnArchivo() const {
        std::wofstream archivo("usuarios.dat", std::ios::binary);
        if (archivo.is_open()) {
            NodoUsuario* actual = primero;
            while (actual != nullptr) {
                archivo << actual->usuario.nombre << L" " << actual->usuario.contrasena << std::endl;
                actual = actual->siguiente;
            }
            archivo.close();
        }
    }
};

struct CLIENTE {
    //agregar lo de la cita
    char nombre[100];
    int tel;
    char especie[100];
    char mascota[100];
    string motivo;
    float costo;
    char estatus[20];
    char fecha[20];
    int	hora;

};

struct NODOCI {
    CLIENTE* dato;
    NODOCI* siguiente;
    NODOCI* anterior;
};

struct CITAS {
    NODOCI* origen;
    NODOCI* fin;
}Agenda;

HWND editUsuario, editContrasena;
ListaUsuarios listaUsuarios;  // Ahora la lista es una variable global

LRESULT CALLBACK ProcPrimeraVentana(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ProcSegundoDialogo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcCitas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AgendaCitas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


void IniciarSesion(HWND hwnd);
void RegistrarUsuario(HWND hwnd);
void AgregarCita(CLIENTE* dato);
NODOCI* nuevoNodo(CLIENTE* dato);
CLIENTE* CrearCita(char* nombre, int tel, char* fecha, int	hora, char* especie, char* mascota, string motivo, float costo, char* estatus);
NODOCI* BuscarCita(char* buscar);
CLIENTE* EliminarCita(char* nombre);
void EditarUsuario(HWND hwnd);//puede o no existir


HINSTANCE hInstGlobal = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    /* Declaración: */
    HWND hwnd;
    MSG mensaje;
    hInstGlobal = hInstance;

    hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ProcPrimeraVentana);

    ShowWindow(hwnd, SW_SHOWDEFAULT);

    /* Bucle de mensajes: */
    while (TRUE == GetMessage(&mensaje, 0, 0, 0)) {
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }

    return mensaje.wParam;
}

LRESULT CALLBACK ProcPrimeraVentana(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_INITDIALOG: {
        editUsuario = GetDlgItem(hwnd, IDC_EDIT1);
        editContrasena = GetDlgItem(hwnd, IDC_EDIT2);
    } break;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1:
            IniciarSesion(hwnd);
            break;

        case IDC_BUTTON2:
            RegistrarUsuario(hwnd);
            break;
        }
    } break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
    } break;

    case WM_DESTROY: {
        PostQuitMessage(0);
    } break;
    }

    return 0;
}
INT_PTR CALLBACK ProcSegundoDialogo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
    } return (INT_PTR)TRUE;

    case WM_COMMAND:

        int menuID = LOWORD(wParam);
        switch (menuID) {
        case (ID_MENU_REINICIAR):
        {
            //nada
        }break;
        case (ID_MENU_CITAS):
        {
           DialogBoxW(hInstGlobal, MAKEINTRESOURCEW(IDD_CITAS), hwnd, ProcCitas);
        }break;
        case (ID_MENU_AGENDA):
        {
            DialogBoxW(hInstGlobal, MAKEINTRESOURCEW(IDD_AGENDA), hwnd, AgendaCitas);
        }break;
        case(ID_MENU_SALIR40005):
        {
            PostQuitMessage(0);
        }break;
        case(IDEditar):
        {

        }break;
        case (IDCANCEL):
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        break;

    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        break;
    }

    return (INT_PTR)FALSE;
}
LRESULT CALLBACK ProcCitas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_INITDIALOG: {
        editUsuario = GetDlgItem(hwnd, IDC_EDIT1);
        editContrasena = GetDlgItem(hwnd, IDC_EDIT2);
    } break;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1:
            IniciarSesion(hwnd);
            break;

        case IDC_BUTTON2:
            RegistrarUsuario(hwnd);
            break;
        }
    } break;

    case WM_CLOSE: {
        DestroyWindow(hwnd);
    } break;

    case WM_DESTROY: {
        PostQuitMessage(0);
    } break;
    }

    return 0;
}


void AgregarCita(CLIENTE* dato) {
    NODOCI* nodo = nuevoNodo(dato);
    if (Agenda.origen == NULL) {
        Agenda.origen = nodo;
        Agenda.fin = nodo;
        nodo->siguiente = NULL;
        nodo->anterior = NULL;
    }
    else {
        nodo->siguiente = Agenda.origen;
        Agenda.origen->anterior = nodo;
        Agenda.origen = nodo;
    }
}

void IniciarSesion(HWND hwnd) {
    wchar_t usuario[100], contrasena[100];
    GetDlgItemTextW(hwnd, IDC_EDIT1, usuario, sizeof(usuario) / sizeof(usuario[0]));
    GetDlgItemTextW(hwnd, IDC_EDIT2, contrasena, sizeof(contrasena) / sizeof(contrasena[0]));

    if (listaUsuarios.verificarUsuario(usuario, contrasena)) {
        DialogBoxW(hInstGlobal, MAKEINTRESOURCEW(IDD_INFORMACIONUSUARIO), hwnd, ProcSegundoDialogo);
    }
    else {
        MessageBoxW(hwnd, L"Usuario o contraseña incorrectos. Por favor, regístrese.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void RegistrarUsuario(HWND hwnd) {
    wchar_t usuario[100], contrasena[100];
    GetDlgItemTextW(hwnd, IDC_EDIT1, usuario, sizeof(usuario) / sizeof(usuario[0]));
    GetDlgItemTextW(hwnd, IDC_EDIT2, contrasena, sizeof(contrasena) / sizeof(contrasena[0]));

    listaUsuarios.agregarUsuario(usuario, contrasena);

    MessageBoxW(hwnd, L"Usuario registrado con éxito", L"Éxito", MB_OK | MB_ICONINFORMATION);
}

CLIENTE* EliminarCita(char* nombre) {
    NODOCI* busqueda = BuscarCita(nombre);
    if (busqueda == NULL)
        return NULL;
    if (busqueda == Agenda.origen)
        return EliminarCitaInicio();
    else if (busqueda == Agenda.fin)
        return EliminarCitaFinal();
    NODOCI* indice = busqueda->anterior;

    CLIENTE* dato = busqueda->dato;
    NODOCI* auxiliar = busqueda->siguiente;
    auxiliar->anterior = indice;
    if (indice != NULL)
        indice->siguiente = busqueda->siguiente;
    delete busqueda;
    return dato;
}

NODOCI* BuscarCita(char* buscar) {
    if (Agenda.origen == NULL)
        return NULL;
    NODOCI* indice = Agenda.origen;
    while (indice != NULL) {
        if (strcmp(indice->dato->nombre, buscar) == 0)
            break;
        indice = indice->siguiente;
    }
    return indice;
}

NODOCI* nuevoNodo(CLIENTE* dato) {
    NODOCI* nodo = new NODOCI;
    nodo->dato = dato;
    nodo->siguiente = NULL;
    nodo->anterior = NULL;
    return nodo;
}

CLIENTE* CrearCita(char* nombre, int tel, char* fecha, int	hora, char* especie, char* mascota, string motivo, float costo, char* estatus) {
    CLIENTE* nuevo = new CLIENTE;
    nuevo->tel = tel;
    nuevo->hora = hora;
    nuevo->costo = costo;
    strcpy_s(nuevo->nombre, 100, nombre);
    strcpy_s(nuevo->mascota, 100, mascota);
    strcpy_s(nuevo->fecha, 20, fecha); //revisar formato
    strcpy_s(nuevo->especie, 100, especie);
    strcpy_s(nuevo->estatus, 20, estatus);
    nuevo->motivo = motivo;
    return nuevo;
}

