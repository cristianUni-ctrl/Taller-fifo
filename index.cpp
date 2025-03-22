// Cristian Arias Mina
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Estructura para definir los nodos
typedef struct Producto {
    int codigo;
    char nombre[50];
    int cantidad;
    int precio;
    struct Producto* siguiente; // Puntero al siguiente nodo
} Producto;

// Estructura para gestionar el inventario
typedef struct Inventario {
    Producto* primero; // Puntero al primer nodo
    Producto* ultimo;  // Puntero al último nodo
} Inventario;

// Inicializar el inventario
void inicializarInventario(Inventario* inv) {
    inv->primero = NULL;
    inv->ultimo = NULL;
}

// Registrar un producto
void registrarProducto(Inventario* inv, int codigo, char* nombre, int cantidad, int precio) {
    Producto* nuevoProducto = (Producto*)malloc(sizeof(Producto));
    if (!nuevoProducto) {
        printf("Error: No se pudo asignar memoria\n");
        return;
    }
    
    nuevoProducto->codigo = codigo;
    strncpy(nuevoProducto->nombre, nombre, sizeof(nuevoProducto->nombre) - 1);
    nuevoProducto->nombre[sizeof(nuevoProducto->nombre) - 1] = '\0';
    nuevoProducto->cantidad = cantidad;
    nuevoProducto->precio = precio;
    nuevoProducto->siguiente = NULL;

    if (inv->primero == NULL) {
        inv->primero = nuevoProducto;
        inv->ultimo = nuevoProducto;
    } else {
        inv->ultimo->siguiente = nuevoProducto;
        inv->ultimo = nuevoProducto;
    }

    printf("Producto registrado correctamente.\n");
}

// Mostrar todos los productos
void mostrarProductos(Inventario* inv) {
    Producto* actual = inv->primero;
    if (actual == NULL) {
        printf("El inventario está vacío.\n");
        return;
    }

    printf("\n--- Lista de Productos ---\n");
    while (actual != NULL) {
        printf("Código: %d, Nombre: %s, Cantidad: %d, Precio: %d, Costo Total: %d\n",
               actual->codigo, actual->nombre, actual->cantidad, actual->precio, actual->cantidad * actual->precio);
        actual = actual->siguiente;
    }
}

// Actualizar la cantidad de un producto tras una venta
void actualizarCantidad(Inventario* inv, int codigo, int cantidadVendida) {
    Producto* actual = inv->primero;
    while (actual != NULL) {
        if (actual->codigo == codigo) {
            if (actual->cantidad >= cantidadVendida) {
                actual->cantidad -= cantidadVendida;
                printf("Venta registrada. Nueva cantidad: %d\n", actual->cantidad);
            } else {
                printf("No hay suficiente stock para la venta.\n");
            }
            return;
        }
        actual = actual->siguiente;
    }
    printf("Producto con código %d no encontrado.\n", codigo);
}

// Calcular el costo total del inventario
int calcularCostoTotal(Inventario* inv) {
    int costoTotal = 0;
    Producto* actual = inv->primero;
    while (actual != NULL) {
        costoTotal += actual->cantidad * actual->precio;
        actual = actual->siguiente;
    }
    return costoTotal;
}

// Eliminar un producto del inventario
void eliminarProducto(Inventario* inv, int codigo) {
    Producto* actual = inv->primero;
    Producto* anterior = NULL;

    while (actual != NULL) {
        if (actual->codigo == codigo) {
            if (anterior == NULL) { // Si es el primer nodo
                inv->primero = actual->siguiente;
                if (inv->primero == NULL) { // Si eliminamos el único nodo
                    inv->ultimo = NULL;
                }
            } else {
                anterior->siguiente = actual->siguiente;
                if (actual == inv->ultimo) { // Si eliminamos el último nodo
                    inv->ultimo = anterior;
                }
            }

            free(actual); // Liberar memoria
            printf("Producto eliminado correctamente.\n");
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    printf("No se encontró el producto con código %d.\n", codigo);
}

// Liberar memoria al salir
void liberarMemoria(Inventario* inv) {
    Producto* actual = inv->primero;
    while (actual != NULL) {
        Producto* temp = actual;
        actual = actual->siguiente;
        free(temp);
    }
    inv->primero = NULL;
    inv->ultimo = NULL;
}

// Función para mostrar el menú
void mostrarMenu() {
    printf("\n--- Menú de Inventario ---\n");
    printf("1. Registrar producto\n");
    printf("2. Mostrar productos\n");
    printf("3. Actualizar cantidad tras venta\n");
    printf("4. Calcular costo total del inventario\n");
    printf("5. Eliminar producto\n");
    printf("6. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    setlocale(LC_ALL, "es_ES.UTF-8");
    Inventario inv;
    inicializarInventario(&inv);
    int opcion, codigo, cantidad, precio;
    char nombre[50];

    while (1) {
        mostrarMenu();
        scanf("%d", &opcion);
        getchar(); // Limpiar buffer de entrada

        switch (opcion) {
            case 1:
                printf("Ingrese el código del producto: ");
                scanf("%d", &codigo);
                getchar();
                printf("Ingrese el nombre del producto: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = 0; // Eliminar el salto de línea
                printf("Ingrese la cantidad del producto: ");
                scanf("%d", &cantidad);
                printf("Ingrese el precio por unidad: ");
                scanf("%d", &precio);
                registrarProducto(&inv, codigo, nombre, cantidad, precio);
                break;
            case 2:
                mostrarProductos(&inv);
                break;
            case 3:
                printf("Ingrese el código del producto vendido: ");
                scanf("%d", &codigo);
                printf("Ingrese la cantidad vendida: ");
                scanf("%d", &cantidad);
                actualizarCantidad(&inv, codigo, cantidad);
                break;
            case 4:
                printf("Costo total del inventario: %d\n", calcularCostoTotal(&inv));
                break;
            case 5:
                printf("Ingrese el código del producto a eliminar: ");
                scanf("%d", &codigo);
                eliminarProducto(&inv, codigo);
                break;
            case 6:
                printf("Saliendo del programa...\n");
                liberarMemoria(&inv);
                return 0;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    }

    return 0;
}
