package paquete.instrumento;

public class Laud extends InstrumentoDeCuerdas {

	public Laud () {
		this.cantCuerdas=0;
	}

	@Override
	public String tocar() {
		return "Suena Laud";
	}

}
