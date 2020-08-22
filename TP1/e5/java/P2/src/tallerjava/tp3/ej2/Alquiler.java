package tallerjava.tp3.ej2;

import java.util.Date;

public class Alquiler {

	private Date fechaAlquiler;
	private Date fechaDevolucion;
	private Alquilable alquilable;
	
	public Date getFechaAlquiler() {
		return fechaAlquiler;
	}
	
	public void setFechaAlquiler(Date fechaAlquiler) {
		this.fechaAlquiler = fechaAlquiler;
	}
	
	public Date getFechaDevolucion() {
		return fechaDevolucion;
	}
	
	public void setFechaDevolucion(Date fechaDevolucion) {
		this.fechaDevolucion = fechaDevolucion;
	}
	
	public void setAlquilable(Alquilable alquilable) {
		this.alquilable = alquilable;
	}
	
	public Alquilable getAlquilable() {
		return alquilable;
	}
	
	public Alquiler() {
		this.fechaAlquiler = new Date();
	}
}
