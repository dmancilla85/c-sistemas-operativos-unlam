package paquete.instrumento;

import paquete.afinador.Afinable;


public class Oboe extends InstrumentoDeViento implements Afinable{

	@Override
	public String tocar() {
		return "Suena Oboe";
	}
	
	public void afinar() {
		System.out.println("Espera que afino el Oboe");
	}

}
