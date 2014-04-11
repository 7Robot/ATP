"
Le robot pouvant partir d’un coté ou de l’autre du plateau, le rôle de cette
fonction est de permettre à l’IA de toujours considérer partir du même coté en
modifiant les paquets pour les « symmétriser ».
Par exemple, une rotation à droite devient une rotation à gauche.
"

def symmetrical(proto, name, args):

    if proto == "asserv":
        if name == "pos" or name == "setPos":
            args["x"] = str(-int(args["x"]))

        elif name == "angle" or name == "setAngle":
            args["theta"] = str(-int(args["theta"]))

        elif name == "omega" or name == "setOmega":
            args["omega"] = str(-int(args["omega"]))

        elif name == "getSICKValue" or name == "SICKValue" or name == "setThreshold":
            # convention : 0 avantG, 1 avanT, 2 arrière
            if args["id"] == "0":
                args["id"] = "1"
            elif args["id"] == "1":
                args["id"] = "0"
