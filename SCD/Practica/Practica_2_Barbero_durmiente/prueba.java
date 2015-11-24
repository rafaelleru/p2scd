import monitor.* ;
import java.util.Random;

class Estanco extends AbstractMonitor {
   private int ingrediente=-1; //Mesa vacia o no
   private Condition obtener_tabaco=makeCondition();
   private Condition obtener_cerillas=makeCondition();
   private Condition obtener_papel=makeCondition();
   private Condition espera_estanquero= makeCondition();  

   public void ObtenerIngrediente(int mi_ingrediente){
      enter();
      if(ingrediente!=mi_ingrediente){
         if(mi_ingrediente==0){
            System.out.println("Fumador 1 espera tabaco");
            obtener_tabaco.await();
         }
         else if(mi_ingrediente==1){
            System.out.println("Fumador 2 espera cerillas");
            obtener_cerillas.await();
         }
         else if(mi_ingrediente==2){
             System.out.println("Fumador 3 espera papel");
             obtener_papel.await();
         }
      }

      if(ingrediente==mi_ingrediente){
         if(mi_ingrediente==0){
            System.out.println("Fumador 1 recoge tabaco");
         }
         else if(mi_ingrediente==1){
            System.out.println("Fumador 2 recoge cerillas");
         }
         else if(mi_ingrediente==2){
             System.out.println("Fumador 3 recoge papel");
         }
      }
     
     ingrediente=-1;
     espera_estanquero.signal();
     leave();
    }

    public void ponerIngrediente(int ingrediente_nuevo){
       enter();
       ingrediente=ingrediente_nuevo;
       if(ingrediente==0) obtener_tabaco.signal();
       if(ingrediente==1) obtener_cerillas.signal();
       if(ingrediente==2) obtener_papel.signal();
       leave();
    }

    public void esperar_recogida(){
       enter();
       if(ingrediente!=-1) esperar_estanquero.await();

       System.out.println("El estanquero ve que esta vacia la mesa locoo");
    }
}




