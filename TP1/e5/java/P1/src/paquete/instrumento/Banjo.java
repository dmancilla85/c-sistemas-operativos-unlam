package paquete.instrumento;

import paquete.afinador.Afinable;


public class Banjo extends InstrumentoDeCuerdas implements Afinable{

	@Override
	public String tocar() {
		return "Suena Banjo";
	}

	@Override
	public void afinar() {
		System.out.println("Espera que afino el Banjo");
	}

}
