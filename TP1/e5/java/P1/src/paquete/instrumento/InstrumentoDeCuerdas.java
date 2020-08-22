package paquete.instrumento;

public abstract class InstrumentoDeCuerdas extends Instrumento {
	
	protected int cantCuerdas;
	
	public int getCantCuerdas() {
		return this.cantCuerdas;
	}
	public void setCantCuerdas(int cantCuerdas) {
		this.cantCuerdas = cantCuerdas;
	}
}
