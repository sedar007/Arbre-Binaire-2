
#include <iostream>

using namespace std;

struct Noeud{
    string nom; 
    int nb_fils;
    Noeud *(*fils); //Ensemble des fils du noeud
};

Noeud *initNoeud(){ // init mon noeud
    Noeud *noeud = new Noeud;
    return noeud;
}

void createArbre(Noeud **noeud){
    //creation de mon arbre de maniere en profondeur
    if( *noeud != nullptr){
        
        cout<<"Nom?  ";
        cin>> (*noeud)->nom;

        cout<<"Saisir le nombre de fils de "<<(*noeud)->nom<<" ";
        cin>> (*noeud)->nb_fils;
        
         (*noeud)->fils = new Noeud*[(*noeud)->nb_fils];

        for(int i=0; i<(*noeud)->nb_fils; i++){
            (*noeud)->fils[i] = new Noeud;
            createArbre(&(*noeud)->fils[i]);    
        }
 
    }
}

void afficher(Noeud *noeud, string tab){
    //Affiche les valeurs en profondeur
    if (noeud != nullptr){
        cout<<noeud->nom<<" a "<<noeud->nb_fils<<" enfants"<<endl;
        tab +="\t";
        
        //Affiche les valeurs des fils jusqu'a arrive ou il y a plus de fils 
        for(int i=0; i< noeud->nb_fils; i++){
            cout<<tab;
            afficher(noeud->fils[i], tab); 
        }        
    }
}

Noeud *rechercherParent(Noeud *noeud, string nom,Noeud *ptr){ // ptr a null a l'appel de la fonction  // recherche le parent
    for(int i = 0; i < noeud->nb_fils; i++){
        if(noeud->fils[i]->nom == nom) return noeud; // si il a un parent, om retourne le parent
        else ptr = rechercherParent(noeud->fils[i], nom,ptr);     
    }
    return ptr; // sinon on retourne nullptr
} 

Noeud *rechercherGrandParent(Noeud *noeud, string nom,Noeud *ptr, Noeud *GrandParent){   // ptr et GrandParent a null a l'appel de la fonction  // recherche le parent
    for(int i = 0; i < noeud->nb_fils; i++){
        if(noeud->fils[i]->nom == nom) return GrandParent;  //si il a un grandParend, om retourne le GrandParent
        else ptr = rechercherGrandParent(noeud->fils[i], nom,ptr, noeud); 
    }
    return ptr;
} 

Noeud *rechercherGrandParent2(Noeud *noeud, string nom){   // ptr et GrandParent a null a l'appel de la fonction  // recherche le parent
    Noeud *pere = rechercherParent(noeud,nom,nullptr);
    Noeud *grandPere;
    if (pere != nullptr) {
        grandPere = rechercherParent(noeud,pere->nom,nullptr);
        return grandPere;
    }
    else return nullptr;
} 

void afficherCousins(Noeud *noeud, string nom, Noeud *fils){ 
    Noeud *GrandParendNoeud = rechercherGrandParent(noeud, nom,nullptr,nullptr);// recherche le grand Perer de noeud
    Noeud *ParendNoeud = rechercherParent(noeud, nom,nullptr); // recherche le pere de noeud

    Noeud *GrandParentFIls = rechercherGrandParent(noeud, fils->nom,nullptr,nullptr); // recherche le grand pere du fils
    Noeud *ParentFils= rechercherParent(noeud, fils->nom,nullptr);// recherche le pere du fils
    
    if(GrandParendNoeud != nullptr && GrandParentFIls != nullptr && GrandParendNoeud == GrandParentFIls){ // verifie si ils ont les memes grand-peres
        if (ParendNoeud != ParentFils) cout<<fils->nom<<" "; // Affiche si ils ne sont pas freres et soeurs
    }

    for(int i=0; i<fils->nb_fils; i++) afficherCousins(noeud, nom, fils->fils[i]); 
    
}

void afficherFreres(Noeud *noeud, string nom, Noeud *fils){
    Noeud *ParendNoeud = rechercherParent(noeud, nom,nullptr);// recherche le pere du noeud
    Noeud *ParentFils= rechercherParent(noeud, fils->nom,nullptr);// recherche le pere du fils

    if (ParendNoeud != nullptr && ParentFils != nullptr && fils->nom != nom && ParendNoeud == ParentFils) cout<<fils->nom<<" "; // Verifie si ils ont le meme pere
    for(int i=0; i<fils->nb_fils; i++) afficherFreres(noeud, nom, fils->fils[i]);

}

Noeud *rechercherNoeud(Noeud *noeud, string nom, Noeud *ptr){ // recherche le noeud dont le nom est passe en parametre
    if(noeud->nom == nom) return noeud; // si c'est le noued en le retourne
    else{
        for(int i=0; i<noeud->nb_fils; i++) ptr = rechercherNoeud(noeud->fils[i], nom, ptr);  
    } 
    return ptr; // sinon on return nullptr/ ptr - nullptr
}

void rajouterEnfant(Noeud *noeud, string nom){ // Permet de rajouter un enfant et ses descendents
    Noeud *parent = rechercherNoeud(noeud,  nom, nullptr); // recherche le noeud du nom

    if(parent == nullptr) return; // Si le nom n'existe pas, on quitte la fonction
    
    Noeud *child = initNoeud(); // Creation d'un nouveau noeud

    parent->nb_fils++; // augement le nombre de fils du noeud
    Noeud *(*new_fils) = new Noeud*[parent->nb_fils]; // creation d'un nouveau tableau de fils

    for(int i=0; i<parent->nb_fils - 1; i++) new_fils[i] = parent->fils[i]; // Remplir ce tableau avec les fils de l'acien
    new_fils[parent->nb_fils - 1] = child; // Ajoute l'adresse du fils creer au tableau de fils

    delete[] parent->fils; // Efface l'ancien tableau

    parent->fils = new_fils; // Donne a mon noeud l'adresse du tableau des ses fils

    createArbre(&child); // Creation du fils est ses descendants

}

int main(){
    bool START = true;
    Noeud *Arbre = initNoeud();
    createArbre(&Arbre);

    cout<<"L'arbre genealogie"<<endl;
    afficher(Arbre,"");cout<<endl;

    while(START){
        int choix;
        string nom;
        Noeud *ptr;

        cout<<"1. Afficher le Grand-Parent"<<endl;
        cout<<"2. Afficher le pareant"<<endl;
        cout<<"3. Afficher les freres et soeurs"<<endl;
        cout<<"4. Afficher les cousins"<<endl;
        cout<<"5. Rajouter un enfant et ses descendants"<<endl;
        cout<<"6. Afficher l'arbre genealogie"<<endl;
        cout<<"Appuyer sur n'importe quel autre bouton pour quitter"<<endl;
        cout<<">> ";
        cin >>choix;

        switch (choix){
            case 1:
                cout<<"Entrer le nom? "<<endl;
                cin>>nom;
                ptr = rechercherGrandParent(Arbre,nom, nullptr, nullptr);
                if(ptr != nullptr) cout<<"Son grand parent est: "<<ptr->nom<<endl;
                else cout<<"Il n'a pas de grand parent"<<endl;
                break;
            
            case 2:
                cout<<"Entrer le nom? "<<endl;
                cin>>nom;
                ptr = rechercherParent(Arbre,nom, nullptr);
                if(ptr != nullptr) cout<<"Son parent est: "<<ptr->nom<<endl;
                else cout<<"Il n'a pas de parent"<<endl;
                break;

            case 3:
                cout<<"Entrer le nom? "<<endl;
                cin>>nom;
                cout<<"Les freres et soeurs sont: ";
                afficherFreres(Arbre, nom, Arbre); cout<<endl;
                break;
            
            case 4:
                cout<<"Entrer le nom? "<<endl;
                cin>>nom;
                cout<<"Les cousins sont: ";
                afficherCousins(Arbre, nom, Arbre); cout<<endl;
                break;

            case 5:
                cout<<"Entrer le nom? "<<endl;
                cin>>nom;
                rajouterEnfant(Arbre,  nom);
                break;

            case 6:
                afficher(Arbre,"");cout<<endl;
                break;

            default:
                START = false;
                break;  
        }  
    }
    
    
}