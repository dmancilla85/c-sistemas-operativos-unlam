package tallerjava.tp3.ej3;

public class Telefono {

	private EstadoTelefono estado=new Colgado();
	
	private void setEstado(EstadoTelefono estado) {
		this.estado = estado;
	}

	public EstadoTelefono getEstado() {
		return estado;
	}

	public void atender(){
		this.setEstado(estado.atender());
	}
	
	public void cortar(){
		this.estado = new Colgado();
	}
}
