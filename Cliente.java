include Barberia;
include Barbero;
include aux;

class Cliente implements Runnable{
  public Thread t;
  private Barberia b;
  int num_cliente;
  aux aux;

  public Cliente(Barberia b, int n){
    b=b;
    num_cliente=n;
    t = new Thread(this, "Cliente", num_cliente);
  }

  public void run(){
    while(true){
      b.CortarPelo(num_cliente);
    }
  }
}
