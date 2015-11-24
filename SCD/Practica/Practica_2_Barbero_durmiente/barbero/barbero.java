import monitor.* ;
import java.util.Random;

class Barberia extends AbstractMonitor {
	Condition espera_silla=makeCondition();
	Condition espera_cliente=makeCondition();
	Condition espera_barbero=makeCondition();

	public void cortarPelo(int numC){
	enter();
	if (!espera_silla.isEmpty() || !espera_cliente.isEmpty()){
	   espera_cliente.await();
           System.out.println("El cliente espera su turno -->"+numC);
        }
        if(!espera_barbero.isEmpty()){
           System.out.println("El cliente despierta al barbero-->"+numC);
           espera_barbero.signal();
        }
        System.out.println("El cliente se va a pelar-->"+numC);
    	if(espera_cliente.isEmpty())
	   espera_silla.await();
        System.out.println("El cliente se despide del barbero -->"+numC);
	leave();
	}

	public void siguienteCliente(){
	enter();
	if(espera_silla.isEmpty()){
		if(espera_cliente.isEmpty()){
                   System.out.println("El barbero se duerme en la silla");
		   espera_barbero.await();
                }
		else {
                   System.out.println("El barbero llama al cliente");
		   espera_cliente.signal();
                }
	}
	leave();
	}

	public void finCliente(){
        enter();
        if(!espera_silla.isEmpty()){
           System.out.println("El barbero ha terminado de pelar al cliente");
	   espera_silla.signal();
        }
        leave();
	}
}	

class Cliente implements Runnable{
    public Thread thr;
    private Barberia barberia;
    int numC;
    aux aux;

    public Cliente(Barberia barberia, int numC){//constructor
        this.barberia = barberia;
        this.numC = numC;
        this.thr = new Thread(this, "Cliente " + numC);
    }

    public void run(){
        while(true){
            barberia.cortarPelo(numC);//cortandose el pelo
        }
    }
}

class Barbero implements Runnable{
    public Thread thr;
    private Barberia barberia;
    aux aux;

    public Barbero(Barberia barberia){
        this.barberia = barberia;
        this.thr = new Thread(this, "Barbero");
    }

    public void run(){
        while(true){
            barberia.siguienteCliente();//llama a siguiente cliente
            barberia.finCliente();//termina de pelarlo
            aux.dormir_max(1000);
        }
    }
}

class aux{
  static Random genAlea = new Random() ;
  static void dormir_max( int milisecsMax )
  { try
    { Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    } 
    catch( InterruptedException e )
    { System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}


class barb{
    public static void main( String[] args ){
      

       
        Cliente[] clientes = new Cliente[3] ;
        Barberia barberia = new Barberia();
        // crear hebra barbero
        Barbero barbero = new Barbero(barberia);

        // crear hebras clientes
        System.out.println("Tamaño vector -->"+clientes.length);
        for(int i = 0; i < clientes.length; i++)
	    clientes[i] = new Cliente(barberia, i) ;

        // poner en marcha las hebras
        barbero.thr.start();
        for(int i = 0; i < clientes.length; i++)
            clientes[i].thr.start();
    }
}


