#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include "heap.h"
//#include "heap.c"
//#include "list_answer.c"
#include "list.h"
#include"stack.h"

typedef struct
{
  char nomTarea[Max];
  int prioridad;
  char precedente[Max];
  Stack* acciones;
}Tarea;

typedef struct
{
  char accion[Max];
  void* dato;
}Historial;

/*
La funcion opcion1 permite agregar tareas a la aplicación. Recibiendo el Heap, el nombre de la tarea y su prioridad, se
comprueba y se reserva memoria (si es necesario). Se crean variables temporales para Tarea e Historial, se asignan los
datos nuevos y se van guardando los datos en los struct correspondientes.
*/
void opcion1(Heap* heapTar, char* nombre, int priori)
{
  if (heapTar->size >= heapTar->capac)
  {
    heapTar->capac = (heapTar->capac * 2) + 1;
    heapTar->heapArray = realloc(heapTar->heapArray, heapTar->capac * sizeof(heapElem));
  }

  Tarea* nuevaTarea = malloc(sizeof(Tarea));
  Historial* historial = malloc(sizeof(Historial));
  
  strcpy(nuevaTarea->nomTarea, nombre);
  nuevaTarea->prioridad = priori;
  strcpy(nuevaTarea->precedente, "");
  nuevaTarea->acciones = stack_create();  // Crear una pila para las acciones

  strcpy(historial->accion, "opcion1");
  historial->dato = (void*)nuevaTarea;
  stack_push(nuevaTarea->acciones, (void*)historial);

  heapTar->heapArray[heapTar->size].data = (void*)nuevaTarea;
  heapTar->heapArray[heapTar->size].priority = priori;
  heapTar->size++;
}

/*
La funcion opcion2 permite establecer la precedencia entre dos tareas. Recibiendo solo el Heap y se le pide al usuario que
ingrese la tarea que debe realizar primero y la que se debe realizar despues, luego se comprueba que ambas tareas se
encuentren en el Heap y se le asigna el precedente correspondiente a la tarea ingresada de ultimas, finalmente se agrega
la accion que se hizo en el struct Historial. 
*/ 
void opcion2(Heap* heapTar)
{
  char nombreTarea1[Max];
  char nombreTarea2[Max];

  printf("\nIngrese el nombre de la primera tarea a realizar: ");
  scanf("%s", nombreTarea1);
  printf("\nIngrese el nombre de la tarea siguiente: ");
  scanf("%s", nombreTarea2);

  int indiceTarea1 = -1;
  int indiceTarea2 = -1;

  for (int i = 0; i < heapTar->size; i++)
  {
    Tarea* tarea = (Tarea*)heapTar->heapArray[i].data;
    
    if (strcmp(tarea->nomTarea, nombreTarea1) == 0)
    {
      indiceTarea1 = i;
    }
    
    if (strcmp(tarea->nomTarea, nombreTarea2) == 0)
    {
      indiceTarea2 = i;
    }
  }

  if (indiceTarea1 == -1 || indiceTarea2 == -1)
  {
    printf("Se ha producido un error.\n");
    return;
  }

  Tarea* tarea2 = (Tarea*)heapTar->heapArray[indiceTarea2].data;
  Historial* historial = (Historial*)malloc(sizeof(Historial));
  
  strcpy(historial->accion, "opcion2");
  historial->dato = tarea2;
  stack_push(tarea2->acciones, historial);
  strcpy(tarea2->precedente, nombreTarea1);
  printf("\nLa tarea '%s' debe realizarse antes que la tarea '%s'.\n", nombreTarea1, nombreTarea2);
}

void opcion3(Heap* tarea)
{
  // Crear un arreglo auxiliar para almacenar las tareas
  Tarea** tareasOrdenadas = malloc(tarea->size * sizeof(Tarea*));

  if (tareasOrdenadas == NULL)
  {
    printf("Se ha producido un error de memoria.");
    return;
  }

  // Copiar las tareas del heap al arreglo auxiliar
  for (int i = 0; i < tarea->size; i++)
  {
    tareasOrdenadas[i] = (Tarea*)tarea->heapArray[i].data;
  }

  // Ordenar el arreglo de tareas por prioridad y precedencia usando Bubble Sort
  for (int i = 0; i < tarea->size - 1; i++)
  {
    for (int j = 0; j < tarea->size - i - 1; j++)
    {
      Tarea* tarea1 = tareasOrdenadas[j];
      Tarea* tarea2 = tareasOrdenadas[j + 1];

      // Comparar prioridades
      if (tarea1->prioridad > tarea2->prioridad)
      {
        // Intercambiar las tareas si la prioridad es mayor
        Tarea* temp = tareasOrdenadas[j];
        tareasOrdenadas[j] = tareasOrdenadas[j + 1];
        tareasOrdenadas[j + 1] = temp;
      }
      else if (tarea1->prioridad == tarea2->prioridad)
      {
        // Si las prioridades son iguales, verificar precedencia
        if (strcmp(tarea1->precedente, tarea2->precedente) > 0)
        {
          // Intercambiar las tareas si la precedencia es mayor
          Tarea* temp = tareasOrdenadas[j];
          tareasOrdenadas[j] = tareasOrdenadas[j + 1];
          tareasOrdenadas[j + 1] = temp;
        }
      }
    }
  }

  printf("\nTareas por hacer, ordenadas por prioridad y precedencia:");
  puts("\n======================================================");

  for (int i = 0; i < tarea->size; i++)
  {
    if(i < 9) printf(" ");
    Tarea* mostrar = tareasOrdenadas[i];

    if (strcmp(mostrar->precedente, "") == 0)
    {
      printf("%i. %s (Prioridad: %i)\n", i + 1, mostrar->nomTarea, mostrar->prioridad);
    }
    else
    {
      printf("%i. %s (Prioridad: %i) - Precedente: %s\n", i + 1, mostrar->nomTarea, mostrar->prioridad, mostrar->precedente);
    }
  }

  puts("======================================================");

  free(tareasOrdenadas); // Liberar la memoria del arreglo auxiliar
}




/*
La funcion opcion4 permite marcar una tarea como completada. Recibe el Heap y el nombre de la tarea que se quiere marcar,
se crea una variable para confirmar que el usuario quiere marcar la tarea como completa. Se busca la tarea en el Heap y se
pregunta si la tarea tiene precedente, si la tarea no tiene precedente esta se borra y se imprime que "la tarea ha sido
marcada como...", en caso contrario, se le pregunta al usuario si esta seguro que quiere marcar (eliminar) la tarea y si
la respuesta es un si, se hace lo mismo que se hizo anteriormente y si la respues es un no, no se hace nada y se imprime
"La tarea no ha...". Finalmente se guarda en el struct Historial la accion que se realizó.
*/
void opcion4(Heap *tarea, char *nombre) 
{
  char opci[3];

  for (int i = 0; i < tarea->size; i++)
  {
    Tarea *buscarT = tarea->heapArray[i].data;

    if(strcmp(buscarT->nomTarea, nombre) == 0)
    {
      if(strcmp(buscarT->precedente, "") == 0)
      {
        for (int j = i; j < tarea->size - 1; j++)
        {
          tarea->heapArray[j] = tarea->heapArray[j + 1];
        }
        tarea->size--;
        printf("\nLa tarea %s ha sido marcada como completada y eliminada de la lista.\n", nombre);
      }
      else
      {
        printf("\n¿Estás seguro que desea eliminar la tarea? si/no\n");
        scanf("%s", opci);
        getchar();
        
        if (strcmp(opci, "si") == 0)
        {
          for (int j = i; j < tarea->size - 1; j++)
          {
            tarea->heapArray[j] = tarea->heapArray[j + 1];
          }
          tarea->size--;
          printf("\nLa tarea %s ha sido marcada como completada y eliminada de la lista.\n", nombre);
          
        }
        else
        {
          printf("\nLa tarea no ha sido eliminada.\n");
        } 
      }
    }
    Historial* historial = malloc(sizeof(Historial));
    
    strcpy(historial->accion, "opcion4");
    historial->dato = (void*)&buscarT;
    stack_push(buscarT->acciones, historial);
  }
}

/*
La funcion opcion5 permite deshacer la última acción. Recibe el Heap, lo primero se pregunta si se hizo alguna accion y
despues entra en un for para encontar la ultima acción que se realizó. Se accede al struct Historia y se pregunta por
medio de if cual fue la ultima accion que realizó el usuario, dependiendo del if que entre se realiza las funciones
necesarias para desacher la ultima opcion e imprimiendo que tal accion fue desecha.
*/
void opcion5(Heap *tarea)
{
  if (tarea->size == 0)
  {
    puts("\n======================================================");
    printf("         No hay acción que se pueda deshacer.\n");
    puts("======================================================");
    return;
  }

  for (int i = tarea->size - 1; i >= 0; i--)
  {
    Tarea *busTarea = tarea->heapArray[i].data;
    Historial *ultiAccion = stack_top(busTarea->acciones);
    
    if (strcmp(ultiAccion->accion, "opcion4") == 0)
    {
      stack_pop(busTarea->acciones);
      tarea->size++;
      printf("\nTarea marcada, desecha\n");
      return;
    }

    if (strcmp(ultiAccion->accion, "opcion2") == 0)
    {
      stack_pop(busTarea->acciones);
      strcpy(busTarea->precedente, "");
      printf("\nPrecedencia agregada, desecha\n");
      return;
    }

    if (strcmp(ultiAccion->accion, "opcion1") == 0)
    {
      stack_pop(busTarea->acciones);
      free(busTarea);
      tarea->size--;
      printf("\nTarea agregada, desecha\n");
      return;
    }
  }
}

/*
La funcion opcion6 permite cargar los datos de un archivo. Recibe el Heap y un archivo (específico), se abre el archivo
para solo lectura (verificando que se abrio corectamente) y se lee la primera linea del archivo sin hacer nada y pasa a
las siguientes lines creando una variable Tarea para guardar los datos en los lugares que corresponden, cuando se leen
todas las lineas se cierra el archivo.
*/
// Función auxiliar para eliminar espacios en blanco al inicio y final de una cadena
char* trim(char *str)
{
  char *end;

  // Eliminar espacios en blanco al final
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
  {
    end--;
  }
  *(end + 1) = '\0';

  // Eliminar espacios en blanco al inicio
  while (*str && isspace((unsigned char)*str))
  {
    str++;
  }

  return str;
}

// Función para agregar tareas desde un archivo CSV al heap
void opcion6(Heap *tarea, char *archi)
{
  FILE *ta = fopen(archi, "r");
  char line[1024];
  
  if (!ta)
  {
    printf("Se produjo un error");
    return;
  }
  else
  {
    fgets(line, 1024, ta);
    
    while (fgets(line, 1024, ta))
    {
      Tarea *tare = malloc(sizeof(Tarea));
      char *token = strtok(line, ",");
      
      if (token != NULL)
      {
        strcpy(tare->nomTarea, token);
      }
      token = strtok(NULL, ",");
      
      if (token != NULL)
      {
        tare->prioridad = atoi(token);
      }
      token = strtok(NULL, "\n");
      
      if (token != NULL)
      {
        if (strcmp(trim(token), ",") == 0)
        {
          strcpy(tare->precedente, "");
          getchar();
        }
        else
        {
          strcpy(tare->precedente, trim(token));
        }
      }
      heap_push(tarea, tare->nomTarea, tare->prioridad);
    }
    fclose(ta);
  }
}


/*
La funcion menu permite que el usuario interactue con el programa. Recibe el Heap, se muestra las diferentes opciones que
tiene la aplicacion y dependiendo de la opcion que desee el usuario es la funcion que se llama para realizar la operacion
requerida.
*/
void menu(Heap *tarea)
{
  int opcion;
  char nombreT[Max];
  int prioridadT;
  char nomTEli[Max];
      
  printf("Menú\n");
  puts("================================================================");
  printf("\t1.- Agregar tarea\n");
  printf("\t2.- Establecer precedendia entre tareas\n");
  printf("\t3.- Mostrar tareas por hacer\n");
  printf("\t4.- Marcar tarea completada\n");
  printf("\t5.- Deshacer última acción\n");
  printf("\t6.- Cargar datos de tareas desde un archivo de texto\n");
  puts("================================================================");
  printf("\nPor favor, ingrese el número de opción que desea seleccionar:\n");
	scanf("%i", &opcion);

  if(opcion == 1)
  {
    getchar();
    printf("\nIngrese nombre de la tarea: ");
    scanf("%[^\n]s", nombreT);
  
    printf("\nIngrese la prioridad de la tarea: ");
    scanf("%i", &prioridadT);
    getchar();

    opcion1(tarea, nombreT, prioridadT);
  }
  
  if(opcion == 2)
  {
    opcion2(tarea);
  }
  
  if(opcion == 3)
  {
    opcion3(tarea);
  }

  if(opcion == 4)
  {
    getchar();
    printf("\nIngrese nombre de la tarea que desea marcar como completada: ");
    scanf("%[^\n]s", nomTEli);
    
    opcion4(tarea, nomTEli);
  }

  if(opcion == 5)
  {
    opcion5(tarea);
  }

  if(opcion == 6) 
  {
    opcion6(tarea, "tareas.csv");
  }
}

/*
El int crea el Heap de tareas y lo manda a la funcion menu. Se hace un ciclo infinito para que se repita todas las veces
que el usuario quiera, en cada iteracion se le pregunta al usuario si quiere realizar otra opcion y se rompre (break)
cuando el usuario ingresa un no.
*/
int main()
{
  Heap* tareas = createHeap();
  
  menu(tareas);
  
  while(1)
  {
    char neg[3] = "no";
    char opc[3];
    
    printf("\n¿Desea realizar alguna otra operacion? si/no\n");
    scanf("%s", opc);
    getchar();
      
    if(strcmp(opc, neg) == 0)
    {
      free(tareas);
      printf("\nEsperamos verte de nuevo por aquí.");
      break;
    }
    printf("\n");
    menu(tareas);
  }
  return 0;
}