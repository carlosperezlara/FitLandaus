int state[2][8][24][128] = { 0 };

void loadState() {
  ifstream fin;
  int arm, lyr, sen, mpd;

  fin.open("PROBLEMS1.lst");
  for(;;) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 1;
  }
  fin.close();

  fin.open("PROBLEMS2.lst");
  for(;;) {
    fin >> arm;
    if(!fin.good()) break;
    fin >> lyr >> sen >> mpd;
    state[arm][lyr][sen][mpd] = 2;
  }
  fin.close();

  return;
}

void makefile(TString run="19020_19035") {
  loadState();

  ifstream ifi;
  TString file, output;
  double dump;
  output = "#ARM LYR TYP SX SY MX SY MIPH eMIPH MIPLH eMIPLH\n";
  cout << output;
  for(int arm=0; arm!=2; ++arm)
    for(int lyr=0; lyr!=8; ++lyr)
      for(int sen=0; sen!=24; ++sen)
	for(int mpd=0; mpd!=128; ++mpd) {
	  int min=14;
	  int max=82;
	  if(lyr>1) {
	    min = 8;
	    max = 62;
	  }
	  int typ = lyr%2;
	  int sx=get_sensor_xx(sen);
	  int sy=get_sensor_yy(sen);
	  int mx, my;
	  if(typ) {
	    my = mpd%32;
	    mx = mpd/32;
	  } else {
	    mx = mpd%32;
	    my = mpd/32;
	  }
	  double mpv = -9999, empv = 0;
	  double sla = 0, esla = 0;
	  if(state[arm][lyr][sen][mpd]!=1) {
	    file = Form("%s/fit/HI_ARM%d_LYR%d_S%d_M%d_%d_%d.dat",run.Data(),arm,lyr,sen,mpd,min,max);
	    ifi.open( file.Data() );
	    ifi >> dump;
	    if( ifi.good() ) {
	      ifi >> dump >> mpv >> empv;
	    }
	    ifi.close();

	    if(state[arm][lyr][sen][mpd]!=2) {
	      file = Form("%s/fit/LO_ARM%d_LYR%d_S%d_M%d_2_18.dat",run.Data(),arm,lyr,sen,mpd);
	      ifi.open( file.Data() );
	      ifi >> dump;
	      if( ifi.good() ) {
		ifi >> esla;
		sla = dump;
	      }
	      ifi.close();
	    }
	  }
	  output = Form("%d %d %d %d %d %d %d ",arm,lyr,typ,sx,sy,mx,my);
	  cout << output.Data();
	  output = Form("%f %f %f %f\n",mpv,empv,sla,esla);
	  cout << output.Data();
	  ifi.close();
	}
}

int get_sensor_xx(int sensor)
{
  int Sy = get_sensor_yy (sensor);
  int Sx = sensor - 20 + 4*Sy;
  if (Sy > 3) Sx = Sx + 1;
  if ((Sy==2 || Sy==3) && (sensor==10 || sensor==11 || sensor==14 || sensor==15))
    Sx = Sx + 2;

  if (Sx < 0 || Sx > 5) Sx=-1;

  return Sx;
}

int get_sensor_yy(int sensor)
{
  int Sy = -1;
  if (sensor < 0) return Sy;
  else if (sensor < 3) Sy = 5;
  else if (sensor < 8) Sy = 4;
  else if (sensor < 12) Sy = 3;
  else if (sensor < 16) Sy = 2;
  else if (sensor < 21) Sy = 1;
  else if (sensor < 24) Sy = 0;
  return Sy;
}
