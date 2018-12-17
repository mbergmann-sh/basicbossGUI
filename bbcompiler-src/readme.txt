BasicBoss Cross Compiler  (ISO C99) 
------------------------
author : Marco Timm
date   : 28.11.2009
version: v0.2.2
contact: marco-onscreen(at)gmx.de
licence: FREEWARE (siehe Anmerkung)
OS     : getestet unter WinXP und Ubuntu 9.10 (Linux)


Was ist das:
------------
  Der BasicBoss Cross Compiler (bbcompiler) ist ein 6502 emulator, der das BasicBoss V2.4 ausfuehrt um 
  C64-Maschinencode aus Basicprogrammen zu erzeugen. BasicBoss ist ein BasicCompiler für den C64, 
  geschrieben von Thilo Herrmann und Veroeffendlicht 1988 im Markt & Technik Verlag.


Anmerkung:
----------
  Die Rechte am orginal BasicBoss und der BasicBoss Anleitung liegen beim Nachfolger des Markt&Technik Verlags,
  dem WEKA-Verlag. Der Begriff Freeware bezieht sich nur auf den 6502Emulator (bbcompiler.c)      


Anwendung: 
----------
  bbcompiler infile [outfile]
   -infile muss prg-format sein (Basicprogramm) 
   -outfile wird im prg-format geschrieben (Maschinencode)


WICHTIG!!!: 
-----------
  PROTOCOL muss in der 1.Zeile im Quelltext stehen, damit der Compiler eventuelle Fehler ausgeben kann!!!  
  (Quelltext: 0 PfundzeichenPROTOCOL )


  Wenn infile kein Basicprogramm ist wird nur eine leere Huelle erzeugt, aber keine Fehlermeldung ausgegeben.


Files:
------
  bbcompiler.exe           BasicBoss Cross Compiler
  demo.prg                 Demoprogramm in Basic (uncompiliert, auch vor dem compilieren lauffaehig)
  demo.txt                 Quelltext von demo.prg (mit TOK64 erzeugt)  
  compile-demo.bat         demo.prg compilieren (Compilat = +out.prg)
  readme.txt               das lesen sie gerade
  tipps.txt                3 Tipps zum Umgang mit dem Compiler

  src\bbcompiler.c         Quelltext 
  src\boss_abbild.inc      BasicBoss Speicherabbild 
  src\makefile             Makefile zum compilieren unter Linux, mit make starten



  anleitung\BasicBoss.pdf  Basicboss Anleitung als PDF
  anleitung\BasicBoss.txt  Basicboss Anleitung als Textdatei
  anleitung\BASICBOSS.D64  orginal BasicBoss V2.4 Diskette  

  anleitung\disk\          Verzeichnis mit Programmen aus BASICBOSS.D64
                           (einige muessen mit ,8 statt ,8,1 geladen werden)   


History:
--------

  v0.2.2 - Der Compiler kann nun ohne Probleme auch Systemweit eingesetzt werden, da sich das 
           BasicBoss-Speicherabbild jetzt im Quellcode befindet und nicht mehr nachgeladen werden muss.
           Vielen Dank an Marc 'BlackJack' Rintsch für die gemachten Aenderungen. :) 

  v0.2.1 - BasicBoss Anleitung verbessert (OCR Fehler entfernt)
           Danke an einen ungenannten User der einiges an Zeit investiert hat :)

  v0.2   - unterstützt das Speichern in Segmenten (ROUTSTART,DATASTART,PROGSTART)

  v0.1   - erstes release


Haftungsausschluss:
-------------------
  Jeder ist für seine Daten selbst verantworlich. Für gelöschte oder überschriebene Daten 
  übernehme ich keine Haftung.
