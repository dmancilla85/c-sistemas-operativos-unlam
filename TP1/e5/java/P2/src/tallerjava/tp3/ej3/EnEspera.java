package tallerjava.tp3.ej3;

public class EnEspera extends EstadoTelefono {

	@Override
	public EstadoTelefono atender() {
	
		System.out.println("Deje su mensaje.");
		
		return this;
	}

	public String toString(){
		String mensaje="En espera";
		return mensaje;
	}
}
