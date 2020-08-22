package paquete.orquesta;

import java.util.ArrayList;
import java.util.List;

import paquete.instrumento.InstrumentoDeCuerdas;
import paquete.instrumento.InstrumentoDeViento;

public class Orquesta {
	
	private List<InstrumentoDeViento> vientos;
	private List<InstrumentoDeCuerdas> cuerdas;
	
	public Orquesta() {
		this.vientos = new ArrayList<InstrumentoDeViento>();
		this.cuerdas = new ArrayList<InstrumentoDeCuerdas>();
	}

	public List<InstrumentoDeViento> getVientos() {
		return vientos;
	}

	public void setVientos(List<InstrumentoDeViento> vientos) {
		this.vientos = vientos;
	}

	public List<InstrumentoDeCuerdas> getCuerdas() {
		return cuerdas;
	}

	public void setCuerdas(List<InstrumentoDeCuerdas> cuerdas) {
		this.cuerdas = cuerdas;
	}
	
	public boolean add(InstrumentoDeCuerdas obj) {
		
		// Add Validate object
		
		this.cuerdas.add(obj);
		return true;
	}
	
	public boolean add(InstrumentoDeViento obj) {
		
		//Add Validate object
		this.vientos.add(obj);
		return true;
	}
	
	private void tocarVientos() {
		for (InstrumentoDeViento objViento : this.vientos) {
			System.out.println(objViento.tocar());
		}
	}
	
	private void tocarCuerdas() {
		for (InstrumentoDeCuerdas objCuerdas : this.cuerdas) {
			System.out.println(objCuerdas.tocar());
		}
	}
	
	public void tocar() {
		this.tocarCuerdas();
		this.tocarVientos();
	}
}
