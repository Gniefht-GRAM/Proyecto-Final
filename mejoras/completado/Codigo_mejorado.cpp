/*
// Materia: Programacion I, Paralelo 1
// Autor: Grover Rene Apaza Massi
// Fecha creación: 12/06/2024
// Fecha modificacion: 20/06/2024
// Proyecto Final: Empresa Art-Sol Bolivia, ventas y reportes con C++
*/
//LIBRERIAS USADAS PARA EL PROYECTO
#include <iostream> // para el uso estandar cout and cin
#include <fstream>// para  abrir y cerrar archivos
#include <vector> // uso de vectores
#include <string> // uso de string caracteres
#include <ctime> // para random
#include <sstream> // cadena de texto como entrada y salida

using namespace std;

// Declaraciones de funciones
void cargarDatos();
void guardarDatos();
void menuPrincipal();
void gestionProductos();
void gestionVentas();
void gestionClientes();
void generarReportes();
void actualizarInventario();
void autenticacion();

// Estructuras de datos para cada caso
struct Producto {
    int id;
    string nombre;
    string descripcion;
    string talla;
    string color;
    double precio;
    int stock;
    string proveedor;
};

struct Venta {
    int id;
    string fecha;
    int id_cliente;
    vector<pair<int, int>> productos; // par de id de producto y cantidad
    double total;
};

struct Cliente {
    int id;
    string nombre;
    string apellido;
    string email;
    string telefono;
    string direccion;
};

// Variables globales usadas
vector<Producto> productos;
vector<Venta> ventas;
vector<Cliente> clientes;
int idProductoActual = 1;
int idVentaActual = 1;
int idClienteActual = 1;

int main() 
{
    setlocale(LC_ALL, "");
    cargarDatos();
    autenticacion();
    guardarDatos();
    return 0;
}

// 1 Funcion para cargar datos desde archivos 
void cargarDatos() {
    // Cargar productos
    ifstream fileProductos("productos.txt");// abrir un archivo modo lectura
    if (fileProductos.is_open()) {
        productos.clear();// limpia el vector
        Producto producto;
        while (fileProductos >> producto.id >> ws) {// ws consume cada espacio para el siguiente dato a ingresar
            getline(fileProductos, producto.nombre, ',');
            getline(fileProductos, producto.descripcion, ',');
            getline(fileProductos, producto.talla, ',');
            getline(fileProductos, producto.color, ',');
            fileProductos >> producto.precio >> producto.stock >> ws;
            getline(fileProductos, producto.proveedor);
            productos.push_back(producto);// agregando los datos al vector
            if (producto.id >= idProductoActual) {// condiciona para que siempre sea mayor
                idProductoActual = producto.id + 1;
            }
        }
        fileProductos.close();// cerrando el archivo es obligatorio hacerlo
    }

    // Cargar ventas
    ifstream fileVentas("ventas.txt");
    if (fileVentas.is_open()) {
        ventas.clear();
        Venta venta;
        while (fileVentas >> venta.id >> ws) {// mientras se pueda leer correctamente se hace lo siguiente:
            getline(fileVentas, venta.fecha, ',');// lee la cadena
            fileVentas >> venta.id_cliente;
            int numProductos;
            fileVentas >> numProductos;
            venta.productos.clear();
            for (int i = 0; i < numProductos; ++i) {// lee cada producto  n veces  de las ventas
                int prodId, cantidad;
                fileVentas >> prodId >> cantidad;
                venta.productos.push_back(make_pair(prodId, cantidad));// añade 2 datos (pair) el id y la cantidad en este caso
            }
            fileVentas >> venta.total;
            ventas.push_back(venta); // añade la venta
            if (venta.id >= idVentaActual) {
                idVentaActual = venta.id + 1;// actualizar los datos de ventas pa que sean unicos
            }
        }
        fileVentas.close();//cerrar el archivo
    }

    // Cargar clientes
    ifstream fileClientes("clientes.txt");
    if (fileClientes.is_open()) {
        clientes.clear();
        Cliente cliente;
        while (fileClientes >> cliente.id >> ws) {
            getline(fileClientes, cliente.nombre, ',');
            getline(fileClientes, cliente.apellido, ',');
            getline(fileClientes, cliente.email, ',');
            getline(fileClientes, cliente.telefono, ',');
            getline(fileClientes, cliente.direccion);
            clientes.push_back(cliente);
            if (cliente.id >= idClienteActual) {
                idClienteActual = cliente.id + 1;
            }
        }
        fileClientes.close();
    }
}


// 2 Funcion para guardar datos en archivos
void guardarDatos() {
    // Guardar productos
    ofstream fileProductos("productos.txt");// para abrir modo escritura datos en un archivo
    if (fileProductos.is_open()) {
        for (const auto &producto : productos) {// (const auto &) evita copiar cada elementopara que no se repita
            fileProductos << producto.id << "\n"
                          << producto.nombre << "\n"
                          << producto.descripcion << "\n"
                          << producto.talla << "\n"
                          << producto.color << "\n"
                          << producto.precio << "\n"
                          << producto.stock << "\n"
                          << producto.proveedor << "\n";
        }
        fileProductos.close();// cerrar el archivo
    } else {// si en caso no lanzar el siguiente mensaje:
        cout << "Error al abrir el archivo de productos para guardar.\n";
    }

    // Guardar ventas
    ofstream fileVentas("ventas.txt");
    if (fileVentas.is_open()) {
        for (const auto &venta : ventas) {
            fileVentas << venta.id << "\n"
                       << venta.fecha << "\n"
                       << venta.id_cliente << "\n"
                       << venta.productos.size() << "\n";
            for (const auto &p : venta.productos) {
                fileVentas << p.first << " " << p.second << "\n";
            }
            fileVentas << venta.total << "\n";
        }
        fileVentas.close();
    } else {
        cout << "Error al abrir el archivo de ventas para guardar.\n";
    }

    // Guardar clientes
    ofstream fileClientes("clientes.txt");
    if (fileClientes.is_open()) {
        for (const auto &cliente : clientes) {
            fileClientes << cliente.id << "\n"
                         << cliente.nombre << "\n"
                         << cliente.apellido << "\n"
                         << cliente.email << "\n"
                         << cliente.telefono << "\n"
                         << cliente.direccion << "\n";
        }
        fileClientes.close();
    } else {
        cout << "Error al abrir el archivo de clientes para guardar.\n";
    }
}

// 3 Funcion para autenticar usuarios
void autenticacion() 
{
    //setlocale(LC_ALL, "");
    string usuario, contra;
    cout << "Ingrese usuario: ";
    cin >> usuario;
    cout << "Ingrese contraseña: ";
    cin >> contra;

    // Aqui se implementa la logica de autenticacion
    menuPrincipal();
}

// Menu principal
void menuPrincipal() {
    int opcion;
    do {// seguira haciendo lo establecido mientras la condicion delfinal de la funcion no se cumpla
        //setlocale(LC_ALL, "");
        cout << "\n----Sistema de Gestión de Inventarios y Ventas ART-SOL BOLIVIA----\n";
        cout << "1. Gestión de Productos\n";
        cout << "2. Gestión de Ventas\n";
        cout << "3. Gestión de Clientes\n";
        cout << "4. Generar Reportes\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {// menu de seleccion
            case 1:
                gestionProductos();
                break;
            case 2:
                gestionVentas();
                break;
            case 3:
                gestionClientes();
                break;
            case 4:
                generarReportes();
                break;
            case 5:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 5);
}

// Funciones de gestion de productos
void gestionProductos() {
    int opcion;
    do {
        //setlocale(LC_ALL, "");
        cout << "\nGestión de Productos\n";
        cout << "1. Alta de Producto\n";
        cout << "2. Baja de Producto\n";
        cout << "3. Modificación de Producto\n";
        cout << "4. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                //setlocale(LC_ALL, "");
                Producto producto;
                producto.id = idProductoActual++;
                cout << "Nombre del producto: ";
                cin >> producto.nombre;
                cout << "Descripción: ";
                cin >> producto.descripcion;
                cout << "Talla: ";
                cin >> producto.talla;
                cout << "Color: ";
                cin >> producto.color;
                cout << "Precio: ";
                cin >> producto.precio;
                cout << "Stock: ";
                cin >> producto.stock;
                cout << "Proveedor: ";
                cin >> producto.proveedor;
                productos.push_back(producto);
                guardarDatos();
                break;
            }
            case 2: {
                int id;
                cout << "Ingrese ID del producto a eliminar: ";
                cin >> id;
                for (auto it = productos.begin(); it != productos.end(); ++it) {// con este for se recorre el vector
                    if (it->id == id) {// comprobar si es igual al ingresado
                        productos.erase(it);//ajusta el vector automaticamente borrando el tamño y actualizando
                        guardarDatos();
                        break;
                    }
                }
                break;
            }
            case 3: {
                int id;
                cout << "Ingrese ID del producto a modificar: ";
                cin >> id;
                for (auto &producto : productos) {
                    if (producto.id == id) {
                        //setlocale(LC_ALL, "");
                        cout << "Nombre del producto: ";
                        cin >> producto.nombre;
                        cout << "Descripción: ";
                        cin >> producto.descripcion;
                        cout << "Talla: ";
                        cin >> producto.talla;
                        cout << "Color: ";
                        cin >> producto.color;
                        cout << "Precio: ";
                        cin >> producto.precio;
                        cout << "Stock: ";
                        cin >> producto.stock;
                        cout << "Proveedor: ";
                        cin >> producto.proveedor;
                        guardarDatos();
                        break;
                    }
                }
                break;
            }
            case 4:
                break;
            default:
                //setlocale(LC_ALL, "");
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 4);
}

// Funciones de gestion de ventas
void gestionVentas() {
    int opcion;
    do {
        //setlocale(LC_ALL, "");
        cout << "\nGestión de Ventas\n";
        cout << "1. Registrar Venta\n";
        cout << "2. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                Venta venta;
                venta.id = idVentaActual++;// valor actual y lo empieza a incrementar
                cout << "ID del cliente: ";
                cin >> venta.id_cliente;

                int idProducto, cantidad;
                while (true) {
                    cout << "Ingrese ID del producto (0 para finalizar): ";
                    cin >> idProducto;
                    if (idProducto == 0) break;
                    cout << "Cantidad: ";
                    cin >> cantidad;
                    venta.productos.push_back(make_pair(idProducto, cantidad));
                }

                // Calcular total de la venta
                venta.total = 0;
                for (auto &p : venta.productos) {// recorre cada par del producto vendido
                    for (auto &producto : productos) {//recorre el vector (es como un for anidado)
                        if (producto.id == p.first) {// comprueba que si el producto del producto coinciden
                            venta.total += producto.precio * p.second;// si se cumple  lo suma
                            producto.stock -= p.second; // Actualizar inventario
                        }
                    }
                }

                // Registrar fecha de la venta con esto se realiza automaticamente la fecha mediante la libreria correspondiente
                time_t now = time(0);// funcion que devuelve el tiempo actual con la libreria time
                tm *ltm = localtime(&now);// lo convierte en una estructura con : año, mes etc
                stringstream ss;// declaracion de un objeto  para elvflujo de entrada y salido (eso nos permite)
                ss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;//costruye el formato YY/MM/DD
                venta.fecha = ss.str();//convierte a una cadena de caracteres string

                ventas.push_back(venta);//agrega
                guardarDatos();//guarda
                break;
            }
            case 2:
                break;
            default:
                setlocale(LC_ALL, "");
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 2);
}

// Funciones de gestion de clientes
void gestionClientes() {
    int opcion;
    do {
        setlocale(LC_ALL, "");// para  el uso de ñ y tildes
        cout << "\nGestión de Clientes\n";
        cout << "1. Registrar Cliente\n";
        cout << "2. Modificar Cliente\n";
        cout << "3. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                //setlocale(LC_ALL, "");
                Cliente cliente;
                cliente.id = idClienteActual++;
                cout << "Nombre: ";
                cin >> cliente.nombre;
                cout << "Apellido: ";
                cin >> cliente.apellido;
                cout << "Email: ";
                cin >> cliente.email;
                cout << "Teléfono: ";
                cin >> cliente.telefono;
                cout << "Dirección: ";
                cin >> cliente.direccion;
                clientes.push_back(cliente);
                guardarDatos();
                break;
            }
            case 2: {
                int id;
                cout << "Ingrese ID del cliente a modificar: ";
                cin >> id;
                for (auto &cliente : clientes) {
                    if (cliente.id == id) {
                        setlocale(LC_ALL, "");
                        cout << "Nombre: ";
                        cin >> cliente.nombre;
                        cout << "Apellido: ";
                        cin >> cliente.apellido;
                        cout << "Email: ";
                        cin >> cliente.email;
                        cout << "Teléfono: ";
                        cin >> cliente.telefono;
                        cout << "Dirección: ";
                        cin >> cliente.direccion;
                        guardarDatos();
                        break;
                    }
                }
                break;
            }
            case 3:
                break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 3);
}

// Funciones para generar reportes
void generarReportes() {
    int opcion;
    do {
        //setlocale(LC_ALL, "");
        cout << "\nGeneración de Reportes\n";
        cout << "1. Reporte de Ventas Diarias\n";
        cout << "2. Reporte de Inventarios\n";
        cout << "3. Reporte de Clientes\n";
        cout << "4. Reporte de Facturación\n";
        cout << "5. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Reporte de Ventas Diarias\n";
                for (auto &venta : ventas) {
                    cout << "ID Venta: " << venta.id << " Fecha: " << venta.fecha << " Total: $" << venta.total << "\n";
                    cout << "Productos Vendidos:\n";
                    for (auto &p : venta.productos) {
                        for (auto &producto : productos) {
                            if (producto.id == p.first) {
                                cout << " - " << producto.nombre << " (Cantidad: " << p.second << ")\n";
                            }
                        }
                    }
                    cout << "\n";
                }
                break;
            }
            case 2: {
                cout << "Reporte de Inventarios\n";
                for (auto &producto : productos) {
                    cout << "ID Producto: " << producto.id << " Nombre: " << producto.nombre << " Stock: " << producto.stock << "\n";
                }
                break;
            }
            case 3: {
                cout << "Reporte de Clientes\n";
                for (auto &cliente : clientes) {
                    cout << "ID Cliente: " << cliente.id << " Nombre: " << cliente.nombre << " " << cliente.apellido << "\n";
                }
                break;
            }
            case 4: {
                //setlocale(LC_ALL, "");
                cout << "Reporte de Facturación\n";
                double totalFacturacion = 0;
                for (auto &venta : ventas) {
                    totalFacturacion += venta.total;
                }
                cout << "Total Facturado: $" << totalFacturacion << "\n";
                break;
            }
            case 5:
                break;
            default:
                //setlocale(LC_ALL, "");
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 5);
}

// Funcion para actualizar el inventario    
void actualizarInventario() {
    for (auto &venta : ventas) {
        for (auto &p : venta.productos) {
            for (auto &producto : productos) {
                if (producto.id == p.first) {
                    producto.stock -= p.second;
                }
            }
        }
    }
}