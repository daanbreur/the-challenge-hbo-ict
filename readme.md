# Betaalbare interactieve manier van leren

## Definities
Hoofdgebruiker - Degene die de quiz presenteert
Gebruiker - Degenen die antwoorden zullen invoeren

Device - Apparaat dat wordt gebruikt om de antwoorden in te voeren
Hoofddevice - Apparaat dat is verbonden aan de computer en de andere devices aanstuurt

## Functioneel ontwerp
### Flowchart
```mermaid
flowchart TB
    subgraph HOOFDGEBRUIKER
    POWERON([Zet device aan]) -->
        CONNECT[Open de software] --> SESSION

            end
                SESSION[Start een sessie] --> SEARCH
                    
    subgraph GEBRUIKER
    GSTART([zet device aan]) --> GPAIR
    end
        GPAIR[Device gaat in koppelstand] --> PAIR
    
    SEARCH{Hoofddevice zoekt devices} --> |Device gevonden| PAIR
    
    PAIR[Verbind met device] --> NAME
    subgraph HOOFDGEBRUIKER
        NAME[Geef naam aan device] -->SEARCH
    
    SEARCH --> |Alle devices verbonden| STOPPAIR

    STOPPAIR[Stop met zoeken] -->
        QSTART[Start quiz]
    end




```