include Barbero;
include Barberia;
include Cliente;

class Peluqueros{
  public static void main(String[] args) {
    Cliente[] clientes = Cliente[3];
    Barberia b = new Barberia();
    Barbero pepe = new Barbero(b);

    //creacion de las hebras
    System.out.println("Numero de clientes = " + clientes.size());
    for (int i=0; i < clientes.size(); i++) {
      clientes[i] = new Cliente(b, i);
    }

    pepe.thr.start();
    for (i=0; i < clientes.size(); i++) {
      clientes[i].thr.start();
    }
  }
}
