# Betaalbare interactieve manier van leren

## Definities
Hoofdgebruiker - Degene die de quiz presenteert
Gebruiker - Degenen die antwoorden zullen invoeren

Device - Apparaat dat wordt gebruikt om de antwoorden in te voeren
Hoofddevice - Apparaat dat is verbonden aan de computer en de andere devices aanstuurt

## Functioneel ontwerp
### Flowchart
```mermaid
flowchart TD


subgraph GEBRUIKER
    GSTART([zet device aan]) --> GPAIR
    end
        GPAIR[Device gaat in koppelstand] --> PAIR

    subgraph HOOFDGEBRUIKER
    POWERON([Zet device aan]) -->
        CONNECT[Open de software] --> SESSION


                SESSION[Start een sessie] --> SEARCH
                    
    
    
    SEARCH{Hoofddevice zoekt devices} --> |Device gevonden| PAIR
    
    PAIR[Verbind met device] --> NAME
        NAME[Geef naam aan device] -->SEARCH
    
    SEARCH --> |Alle devices verbonden| STOPPAIR

    STOPPAIR[Stop met zoeken] --> QSTART
    end
        QSTART[Start quiz] --> VRAAG1
    

    subgraph QUIZ
        VRAAG1[Stel vraag] -->
        ANS[Verzamel antwoorden] -->
        RESULT{Resultaat} --> |Volgende vraag|VRAAG1
        RESULT --> |Laatste vraag|EINDE

    end




```