package tallerjava.tp3.ej3;

public class Colgado extends EstadoTelefono {

	@Override
	public EstadoTelefono atender() {
	
		System.out.println("Hola");
		
		EstadoTelefono nuevoEstado = new Ocupado();
		
		return nuevoEstado;
	}

	public String toString(){
		String mensaje="Colgado";
		return mensaje;
	}
}
