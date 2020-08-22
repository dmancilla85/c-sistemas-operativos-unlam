package tallerjava.tp3.ej3;

public class Ocupado extends EstadoTelefono {

	@Override
	public EstadoTelefono atender() {

		System.out.println("Llamada en espera. Sonido por auricular.");
		
		EstadoTelefono nuevoEstado = new EnEspera();
		
		return nuevoEstado;
	}

	public String toString(){
		String mensaje="Ocupado";
		return mensaje;
	}
}
