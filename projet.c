#include <stdio.h>

typedef struct {
      float latitude;
      float longitude;
} Position;
typedef struct {
      Position rpos;
      float vitmax;
} Zone;

Zone zones[] = {
{{44.7887762, -3.012}, 50}, /* Descripteur de la première zone */
{{44.7891220, -3.013}, 70},
};

int decode_int(char c)
{
      int code;
      char i;
      if(c >= '0' && c <= '9')
      // Verification que le caractère 'c' est un caractère entre 0 et 9 dans la table ascii
      {
            for (i= '0'; i <='9';i++)
            //Parcourir la table ascii à la recherche du carac. c
            {
                  if (i==c)
                  // Identification du carac. c dans la table ascii
                  {
                        code=i-48; // On ramène le code ascii de c entre 0 et 9
                        return code;
                  }
            }
      }
      else
      {
            // le caractère 'c' n'est pas entre '0' et '9' dans la table ascii donc on renvoie -1
            return -1;
      }
}
int trame_cmp(char * trame, char * type)
{
      int indice=1;
      char mot[10]="";
      do
      {
            mot[indice-1]=trame[indice];
            indice++;
      } while (trame[indice]!=',');

      int longueurMot=0;
      int longueurType=0;
      char caratereCourrant=0;

      do
      {
            caratereCourrant=mot[longueurMot];
            longueurMot++;
      } while (caratereCourrant != '\0');
      longueurMot--;

      caratereCourrant=0;
      do
      {
            caratereCourrant=type[longueurType];
            longueurType++;
      } while (caratereCourrant != '\0');
      longueurType--;

      if (longueurType==longueurMot)
      {
            int verificateur=0;
            for (int i = 0; i < longueurMot; i++)
            {
                  if (mot[i]==type[i])
                  {
                        verificateur=1;
                  }
                  else
                  {
                        verificateur=0;
                        break;
                  }
            }
            if (verificateur==1)
            {
                  return 1;
            }
            else
            {
                  return 0;
            }
      }
      else
      {
            return 0;
      }
}
int decode_nombre(char * ch, int n)
{
      int longueurMot=0;
      char caratereCourrant=0;
      //Obtention de la longueur du Mot extrait entre le symbole $ et la virgule
      do
      {
            caratereCourrant=ch[longueurMot];
            longueurMot++;
      }
      while (caratereCourrant != '\0');
      longueurMot--;
      if (longueurMot < n ||n==0){
            return 0;
      }
      else{
            int j=0,sum=0;
            for(j=0; j<=n; j++)
            {
                  if(ch[j]=='.')
                  {
                        sum=sum;
                  }
                  else if(ch[j] < 48 || ch[j] > 57)
                  {
                        return 0;
                  }
                  else
                  {
                        sum=sum*10 + decode_int(ch[j]);
                  }
            }
            return sum;
      }
}

float CharVersRadians(char * ch)
{
      char * nombre=ch;
      float dec;
      int compte=0,n,degre;
      float res = 0, fact = 1;
      if (*ch == '-')
      {
            ch++;
            fact = -1;
      }
      for (int virguleVu = 0; *ch; ch++)
      {
            compte++;
            if (*ch == '.')
            {
                  virguleVu = 1;
                  n=compte-1;
                  continue;
            }
            int d = *ch - '0';
            if (d >= 0 && d <= 9)
            {
                  if (virguleVu!=0) 
                  {
                        fact /= 10.0;
                  }
                  res = res * 10.0 + (float)d;
            }
            if (*ch == ',')
            {
                  break;
            }
      }
      dec=(res * fact);
      if (n==4)
      {     
            degre=decode_nombre(nombre,1);
            dec=(dec-degre*100)/60;
      }
      else if (n==5)
      {
            degre=decode_nombre(nombre,2);
            dec=(dec-degre*100)/60;
      }
      return (degre+dec);
}
int CharVersRadiansV2(char * ch)
{
      int resultat=0;
      int longueurCh=0;
      char caratereCourrant=0;
      do
      {
            caratereCourrant=ch[longueurCh];
            longueurCh++;
      }
      while (caratereCourrant != '\0');
      longueurCh--;
          
      if (longueurCh == 9)
      {
            int nombre=decode_nombre(ch,8);
            int nombre_avant_vir=decode_nombre(ch,4);
            int deg=decode_nombre(ch,1);
            int minute_avant_vir=nombre_avant_vir-deg*100;
            //printf("nombre=%d | deg=%d ",nombre,deg);
            int minute_apres_vir=(nombre-nombre_avant_vir*10000);
            //printf("minute_apres_vir=%d | minute_avant_vir=%d \n",minute_apres_vir,minute_avant_vir);

            resultat=(deg+(minute_avant_vir+minute_apres_vir*0.0001)/60)*256;
            //printf("resultat = %d deg | ",resultat);
            //printf("resultat = %f deg \n",result);
      }
      else
      {
            int nombre=decode_nombre(ch,9);
            int nombre_avant_vir=decode_nombre(ch,5);
            int deg=decode_nombre(ch,2);
            int minute_avant_vir=nombre_avant_vir-deg*100;
            //printf("nombre=%d | deg=%d ",nombre,deg);
            int minute_apres_vir=(nombre-nombre_avant_vir*10000);
            //printf("minute_apres_vir=%d | minute_avant_vir=%d \n",minute_apres_vir,minute_avant_vir);

            resultat=(deg+(minute_avant_vir+minute_apres_vir*0.0001)/60)*256;
            //printf("resultat = %d deg | ",resultat);
            //printf("resultat = %f deg \n",result);
      }
      return resultat;
}

Position decode_trameV1(char * ch)
{
      Position pos1;
      pos1.latitude=0.0;
      pos1.longitude=0.0;
      int i,indice=0;
      char longitude[20]="";
      char latitude[20]="";
      int compteur_de_virgule = 0;
      if (trame_cmp(ch,"GPGGA")==0)
      {
            return pos1;
      }
      else
      {
            
            do
            {
                  if (ch[indice]==',')
                  {
                        compteur_de_virgule++;
                  }

                  if (compteur_de_virgule==2)
                  {
                        for (int i = 0; i < 9; i++)
                        {
                              latitude[i]=ch[indice+1];
                              indice++;
                        }
                  }

                  if (compteur_de_virgule==4)
                  {
                        for (int i = 0; i < 10; i++)
                        {
                              longitude[i]=ch[indice+1];
                              indice++;
                        }
                        break;
                  }
                  indice++;          
            } while (ch[indice]!='\0');
            printf("latitude : %s , longitude : %s\n",latitude,longitude);
            pos1.latitude = CharVersRadians(latitude);
            pos1.longitude = CharVersRadians(longitude);
            printf("latitude=%f deg | longitude=%f deg \n",pos1.latitude,pos1.longitude);
      }
      return pos1;
}
Position decode_trameV2(char * ch)
{
      Position pos1;
      pos1.latitude=0.0;
      pos1.longitude=0.0;
      int i,indice=0;
      char longitude[20]="";
      char latitude[20]="";
      int compteur_de_virgule = 0;
      if (trame_cmp(ch,"GPGGA")==0)
      {
            return pos1;
      }
      else
      {
            
            do
            {
                  if (ch[indice]==',')
                  {
                        compteur_de_virgule++;
                  }

                  if (compteur_de_virgule==2)
                  {
                        for (int i = 0; i < 9; i++)
                        {
                              latitude[i]=ch[indice+1];
                              indice++;
                        }
                  }

                  if (compteur_de_virgule==4)
                  {
                        for (int i = 0; i < 10; i++)
                        {
                              longitude[i]=ch[indice+1];
                              indice++;
                        }
                        break;
                  }
                  indice++;          
            } while (ch[indice]!='\0');
            printf("latitude : %s , longitude : %s\n",latitude,longitude);
            pos1.latitude = (float)CharVersRadiansV2(latitude)/256;
            pos1.longitude = (float)CharVersRadiansV2(longitude)/256;
            printf("latitude=%f deg | longitude=%f deg \n",pos1.latitude,pos1.longitude);
      }
      return pos1;
}

float calcul_distanceV1(Position p1,Position p2)
{     
      float R_terre=6378.137;
      float distance = R_terre*(p1.latitude-p2.latitude)*(3.14/180);
      return distance;
}
float calcul_vitesse(float distance)
{
      return distance*3600;
}
float distance_a_la_plus_proche_zone(Position pos_voiture, Zone r[], int nb_zones)
{
      Position t[nb_zones];
      for (int i = 0; i < nb_zones; i++)
      {
            Position pos=r[i].rpos;
            t[i]=pos;
      }
      float d_min=calcul_distance(pos_voiture,t[1]);
      int indice_rechercher=0;
      for (int i = 0; i < nb_zones; i++)
      {
            float d=calcul_distance(pos_voiture,t[i]);
            if (d<d_min)
            {
                  d_min=d;
                  indice_rechercher=i;
            }
            if (nb_zones==0)
            {
                  return -1;
            }
      }
      return d_min;
}

int Equal_Mat_Mat(int Ligne_MatA, int Colonne_MatA, double A[Ligne_MatA][Colonne_MatA], double B[Ligne_MatA][Colonne_MatA])
{
      int valide=0;
      for (int i = 0; i < Ligne_MatA; i++)
      {
            for (int j = 0; j < Colonne_MatA; j++)
            {
                  if (A[i][j]!=B[i][j])
                  {
                        valide=0;
                        break;
                  }
                  
            } 
      } 
      valide=1;
      return valide;
}
void Add_Mat_Mat(int Ligne_MatA, int Colonne_MatA, double A[Ligne_MatA][Colonne_MatA],int Ligne_MatB, int Colonne_MatB, double B[Ligne_MatB][Colonne_MatB], double Res[Ligne_MatA][Colonne_MatA])
{
      for (int i = 0; i < Ligne_MatA; i++)
      {
            for (int j = 0; j < Colonne_MatA; j++)
            {
                  Res[i][j] = A[i][j]+B[i][j];
            } 
      }
}
void Sub_Mat_Mat(int Ligne_MatA, int Colonne_MatA, double A[Ligne_MatA][Colonne_MatA],int Ligne_MatB, int Colonne_MatB, double B[Ligne_MatB][Colonne_MatB], double Res[Ligne_MatA][Colonne_MatA])
{
      for (int i = 0; i < Ligne_MatA; i++)
      {
            for (int j = 0; j < Colonne_MatA; j++)
            {
                  Res[i][j] = A[i][j]-B[i][j];
            } 
      }
}
void Mul_Mat_Mat(int Ligne_MatA, int Colonne_MatA, double A[Ligne_MatA][Colonne_MatA],int Ligne_MatB, int Colonne_MatB, double B[Ligne_MatB][Colonne_MatB], double Res[Ligne_MatA][Colonne_MatA])
{
      for (int i = 0; i < Ligne_MatA; i++)
      {
            for (int j = 0; j < Colonne_MatA; j++)
            {
                  Res[i][j] += A[i][j]*B[j][i];
            } 
      }
}
void Inverse_Mat_Mat(int Ligne_MatA, int Colonne_MatA, double A[Ligne_MatA][Colonne_MatA],double Res[Ligne_MatA][Colonne_MatA])
//OBLIGATOIREMENT POUR DES MATRICES DE TAILLES 2x2
{
      double unSurDet;
      unSurDet = 1/(A[0][0]*A[1][1]-A[0][1]*A[1][0]);
      for (int i = 0; i < Ligne_MatA; i++)
      {
            for (int j = 0; j < Colonne_MatA; j++)
            {
                  if (i==0 && j==1)
                  {
                        Res[i][j]=unSurDet*(-A[i][j]);
                  }
                  else if (i==1 && j==0)
                  {
                        Res[i][j]=unSurDet*(-A[i][j]);
                  }
                  else
                  {
                        Res[i][j]=unSurDet*A[i][j];
                  }
            } 
      }    
}
void Transpose_Mat(int Ligne_MatA, int Colonne_MatA, double A[Ligne_MatA][Colonne_MatA],double Res[Ligne_MatA][Colonne_MatA])
{
	int i,j;
	for (i=0;i<Ligne_MatA;i++)
		for (j=0;j<Colonne_MatA;j++)
			Res[j][i]=A[i][j];
}

void tests_unitaires(void){
      printf("\n Premiere trame \n");
      printf("00306.6039 \n");
      printf("V2 : int = %d | float = %f \n",CharVersRadiansV2("00306.6039"),(float)CharVersRadiansV2("00306.6039")/256);
      printf("V1 : float = %f ",CharVersRadians("00306.6039"));
      printf("\n Deuxieme trame \n");
      printf("4545.1410 \n");
      printf("V2 : int = %d | float = %f \n",CharVersRadiansV2("4545.1410"),(float)CharVersRadiansV2("4545.1410")/256);
      printf("V1 : float = %f ",CharVersRadians("4545.1410"));

      printf("\n TEST DECODE TRAME \n");
      decode_trame("$GPGGA,141920.00,4545.1410,N,00306.6039,E,1,05,3.4,500.0,M,,M,,*79");
      /*
	//Matrices d'entrée
	double T21a[2][1]={{7},{-5}};
	double T21b[2][1]={{-3},{46}};
	double T22a[2][2]={{12,78},{-5,13}};
	double T22b[2][2]={{-25,36},{7,42}};
	double T24[2][4]={{7,-71,-12,3},{41,123,-5,10}};
	double T41a[4][1]={{45},{-123},{-78},{-410}};
	double T41b[4][1]={{-10},{45},{27},{-9}};
	double T42a[4][2]={{-73,45},{10,12},{-41,-35},{8,-23}};
	double T44a[4][4]={{1,2,7,4},{6,5,7,8},{9,8,7,6},{5,4,3,2}};
	double T44b[4][4]={{12,13,14,15},{21,22,23,40},{78,45,12,3},{54,10,12,47}};

	//Matrices résultat
	double R21[2][1],R22[2][2],R24[2][4],R41[4][1],R42[4][2],R44[4][4];

	//Matrices de validation
	double RST21[2][1]={{10},{-51}};
	double RInvT22[2][2]={{0.02380952380952381,-0.1428571428571428},{0.009157509157509158,0.02197802197802198}};
	double RAT22[2][2]={{-13,114},{2,55}};
	double RTT24[4][2]={{7,41},{-71,123},{-12,-5},{3,10}};
	double RMT24T41[2][1]={{8754},{-16994}};
	double RMT24T42[2][2]={{-705,-186},{-1478,3266}};
	double RMT24T44[2][4]={{-512,-425,-523,-606},{784,697,1143,1138}};
	double RAT41[4][1]={{35},{-78},{-51},{-419}};
	double RMT42T21[4][1]={{-736},{10},{-112},{171}};
	double RMT42T22[4][2]={{-1101,-5109},{60,936},{-317,-3653},{211,325}};
	double RMT42T24[4][4]={{1334,10718,651,231},{562,766,-180,150},{-1722,-1394,667,-473},{-887,-3397,19,-206}};
	double RTT44[4][4]={{1,6,9,5},{2,5,8,4},{7,7,7,3},{4,8,6,2}};
	double RMT44T41[4][1]={{-2387},{-4171},{-3585},{-1321}};
	double RMT44T42[4][2]={{-308,-268},{-611,-99},{-816,118},{-432,122}};
	double RAT44[4][4]={{13,15,21,19},{27,27,30,48},{87,53,19,9},{59,14,15,49}};
	double RST44[4][4]={{-11,-11,-7,-11},{-15,-17,-16,-32},{-69,-37,-5,3},{-49,-6,-9,-45}};
	double RMT44T44[4][4]={{816,412,192,304},{1155,583,379,687},{1146,668,466,758},{486,308,222,338}};

	printf("Execution des tests unitaires.\n");
	Transpose_Mat(2,4,T24,R42);		if (!Equal_Mat_Mat(4,2,RTT24,R42)) 		error("Erreur calcul Transposition 2x4");
	Transpose_Mat(4,4,T44a,R44);	if (!Equal_Mat_Mat(4,4,RTT44,R44)) 		error("Erreur calcul Transposition 4x4");
	Inverse_Mat_Mat(2,2,T22a,R22);	if (!Equal_Mat_Mat(2,2,RInvT22,R22)) 	error("Erreur calcul Inversion 2x2");
	Add_Mat_Mat(2,2,T22a,2,2,T22b,R22);	if (!Equal_Mat_Mat(2,2,RAT22,R22)) 		error("Erreur calcul Addition 2x2");
	Add_Mat_Mat(4,4,T44a,4,4,T44b,R44);	if (!Equal_Mat_Mat(4,4,RAT44,R44)) 		error("Erreur calcul Addition 4x4");
	Add_Mat_Mat(4,1,T41a,4,1,T41b,R41);	if (!Equal_Mat_Mat(4,1,RAT41,R41)) 		error("Erreur calcul Addition 4x1");
	Sub_Mat_Mat(2,1,T21a,2,1,T21b,R21);	if (!Equal_Mat_Mat(2,1,RST21,R21)) 		error("Erreur calcul Soustraction 2x1");
	Sub_Mat_Mat(4,4,T44a,4,4,T44b,R44);	if (!Equal_Mat_Mat(4,4,RST44,R44)) 		error("Erreur calcul Soustraction 4x4");
	Mul_Mat_Mat(4,4,T44a,4,4,T44b,R44);	if (!Equal_Mat_Mat(4,4,RMT44T44,R44)) 	error("Erreur calcul Multiplication 4x4 4x4");
	Mul_Mat_Mat(4,4,T44a,4,1,T41a,R41);	if (!Equal_Mat_Mat(4,1,RMT44T41,R41)) 	error("Erreur calcul Multiplication 4x4 4x1");
	Mul_Mat_Mat(4,4,T44a,4,2,T42a,R42);	if (!Equal_Mat_Mat(4,2,RMT44T42,R42)) 	error("Erreur calcul Multiplication 4x4 4x2");
	Mul_Mat_Mat(4,2,T42a,2,1,T21a,R41);	if (!Equal_Mat_Mat(4,1,RMT42T21,R41)) 	error("Erreur calcul Multiplication 4x2 2x1");
	Mul_Mat_Mat(4,2,T42a,2,2,T22a,R42);	if (!Equal_Mat_Mat(4,2,RMT42T22,R42)) 	error("Erreur calcul Multiplication 4x2 2x2");
	Mul_Mat_Mat(4,2,T42a,2,4,T24,R44);	if (!Equal_Mat_Mat(4,4,RMT42T24,R44)) 	error("Erreur calcul Multiplication 4x2 2x4");
	Mul_Mat_Mat(2,4,T24,4,1,T41a,R21);	if (!Equal_Mat_Mat(2,1,RMT24T41,R21)) 	error("Erreur calcul Multiplication 2x4 4x1");
	Mul_Mat_Mat(2,4,T24,4,2,T42a,R22);	if (!Equal_Mat_Mat(2,2,RMT24T42,R22)) 	error("Erreur calcul Multiplication 2x4 4x2");
	Mul_Mat_Mat(2,4,T24,4,4,T44a,R24);	if (!Equal_Mat_Mat(2,4,RMT24T44,R24)) 	error("Erreur calcul Multiplication 2x4 4x4");
	printf("Test unitaires OK.\n");*/
}

void main()
{
     tests_unitaires();
}
