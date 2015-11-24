class Barberia extends AbstractMonitor{
  Condition sala_espera=makeCondition();
	Condition cliente=makeCondition();
	Condition barbero=makeCondition();

  public void CortarPelo(int num_cliente){
    enter;
    if(!sala_espera.isEmpty() || !cliente.isEmpty()){
      cliente.await();
      System.out.println("el cliente esta esperando su turno")
    }

    if(!barbero.isEmpty()){
      System.out.println("el cliente avisa al barbero que se quiere cortar el pelo");
      barbero.signal();
    }

    if (cliente.isEmpty()) {
      sala_espera.await();
    }

    System.out.println("el cliente va a cortase el pelo")
    leave();
  };

  public void SiguienteCliente(){
    enter();
    if(sala_espera.isEmpty()){
      if(cliente.isEmpty()){
        System.out.println("El barbero se duerme");
        barbero.await();
      }else{
        System.out.println("el barbero llama el cliente");
        sala_espera.signal();
      }
    }
    leave();
  };
  public void finCliente(){
    enter();
    if(!espera_silla.isEmpty()){
       System.out.println("El barbero ha terminado de cortar el pelo al cliente");
       espera_silla.signal();
    }
    leave();
  };
}
