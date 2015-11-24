include Barberia;
include Cliente;
include aux;

class Barbero implements Runnable{
  public Thread t;
  private Barberia b;
  aux aux;

  public Cliente(Barberia b){
    b = b;
    t = new Thread(this, "Barbero");
  }

  public void run(){
    while(true){
      b.siguienteCliente();
      b.finCliente();
      aux.dormir_max(1000);
    }
  }
}
