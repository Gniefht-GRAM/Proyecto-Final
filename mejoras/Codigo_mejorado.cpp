/*
// Materia: Programaci�n I, Paralelo 1
// Autor: Grover Rene Apaza Massi
// Fecha creaci�n: 12/06/2024
// Fecha modificaci�n: 20/06/2024
// Proyecto Final: Empresa Art-Sol Bolivia, ventas y reportes con C++
// 
*/
//LIBRERIAS USADAS PARA EL PROYECTO
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>

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
void alertasStockBajo();
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

int main() {
    cargarDatos();
    autenticacion();
    guardarDatos();
    return 0;
}

// Funcion para cargar datos desde archivos
void cargarDatos() {
    // Implementar carga de datos desde archivos (productos.txt, ventas.txt, clientes.txt)
}

// Funcion para guardar datos en archivos
void guardarDatos() {
    // Implementar guardado de datos en archivos (productos.txt, ventas.txt, clientes.txt)
}

// Funcion para autenticar usuarios
void autenticacion() {
    string usuario, contraseña;
    cout << "Ingrese usuario: ";
    cin >> usuario;
    cout << "Ingrese contraseña: ";
    cin >> contraseña;

    // Aquí se implementa la logica de autenticacion
    menuPrincipal();
}

// Menu principal
void menuPrincipal() {
    int opcion;
    do {
        cout << "\nSistema de Gestión de Inventarios y Ventas\n";
        cout << "1. Gestión de Productos\n";
        cout << "2. Gestión de Ventas\n";
        cout << "3. Gestión de Clientes\n";
        cout << "4. Generar Reportes\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
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

// Funciones de gestión de productos
void gestionProductos() {
    int opcion;
    do {
        cout << "\nGestión de Productos\n";
        cout << "1. Alta de Producto\n";
        cout << "2. Baja de Producto\n";
        cout << "3. Modificación de Producto\n";
        cout << "4. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
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
                for (auto it = productos.begin(); it != productos.end(); ++it) {
                    if (it->id == id) {
                        productos.erase(it);
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
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 4);
}

// Funciones de gestion de ventas
void gestionVentas() {
    int opcion;
    do {
        cout << "\nGestión de Ventas\n";
        cout << "1. Registrar Venta\n";
        cout << "2. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                Venta venta;
                venta.id = idVentaActual++;
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
                for (auto &p : venta.productos) {
                    for (auto &producto : productos) {
                        if (producto.id == p.first) {
                            venta.total += producto.precio * p.second;
                            producto.stock -= p.second; // Actualizar inventario
                        }
                    }
                }

                // Registrar fecha de la venta
                time_t now = time(0);
                tm *ltm = localtime(&now);
                stringstream ss;
                ss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
                venta.fecha = ss.str();

                ventas.push_back(venta);
                guardarDatos();
                break;
            }
            case 2:
                break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 2);
}

// Funciones de gestion de clientes
void gestionClientes() {
    int opcion;
    do {
        cout << "\nGestión de Clientes\n";
        cout << "1. Registrar Cliente\n";
        cout << "2. Modificar Cliente\n";
        cout << "3. Regresar al Menú Principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
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
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while (opcion != 5);
}

// Funcion para actualizar el inventario (no utilizada explícitamente, se incluye como referencia)
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

// Función para alertar sobre stock bajo
void alertasStockBajo() {
    for (auto &producto : productos) {
        if (producto.stock < 10) { // Nivel de stock bajo definido como menos de 10 unidades
            cout << "ALERTA: El producto " << producto.nombre << " tiene stock bajo (" << producto.stock << " unidades).\n";
        }
    }
}
