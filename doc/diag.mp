input metauml;

beginfig(1);
iClass.foreColor := (.5, 1, .5);
		
Class.game_obj("game_obj")
		("+TEX: int")
		("+canFall(): bool",
		"+canEnter(): bool",
		"+isSolid(): bool",
		"+isAnimated(): bool",
		"+drawIt(): bool",
		"+typKlasy(): string",
		"+playerEnters(): bool");

Class.Vector("Vector3D")
		("+x: float",
		"+y: float",
		"+z: float")
		("+basic aritmetrics(+,-,*,/,==,!=)");
leftToRight(70)(Vector, game_obj); 

% children of game_obj
Class.animated("animated")
		("+animating: bool")
		("+UpdateAnimation():bool",
		"+isAnimated():bool",
		"+typKlasy():string");
Class.Wall("Wall")
		()
		("+canEnter(delta: Vector3D): bool",
		"+isSolid(): bool",
		"+drawIt(): bool"
		);
Class.Floor("Floor")
		("+stable: bool")
		("+canEnter(delta: Vector3D): bool",
		"+isSolid(): bool",
		"+drawIt(): bool"
		);
Class.EmptySpace("EmptySpace")
		()
		("+canEnter(delta: Vector3D): bool",
		"+UpdateAnimation: bool",
		"+isSolid(): bool",
		"+drawIt(): bool"
		);
Class.Switch("Switch")
		()
		("+drawIt(): bool");
Switch.info.foreColor := (.9,.7,.7);

leftToRight.top(50)(Wall,animated);
leftToRight.top(20)(animated,Floor,EmptySpace,Switch);

Group.game_objChildren(animated,Wall,Floor,EmptySpace,Switch);
topToBottom(30)(game_obj,game_objChildren);

drawObjects(game_obj, Vector, game_objChildren);
% end children of game_obj

% children of animated
Class.moved("moved")
		( "+startPos: Vector3D",
		"+endPos: Vector3D",
		"+startTime:float",
		"+endTime:float")
		("+setAnimation():void",
		"+checkFloor():bool",
		"+UpdateAnimation():bool",
		"+typKlasy():string");
Class.rotateds("rotated")
		("+rotationVector:Vector3D",
		"+angle:float",
		"+accumulator:float")
		("+UpdateAnimation():bool");
Class.scaleds("scaled")
		("+scale:float ",
		"+startScale:float ",
		"+endScale:float ",
		"+startTime:float ",
		"+endTime: float ")
		("+setAnimation():void",
		"+UpdateAnimation():bool");

leftToRight.(100)(moved,rotateds,scaleds);

Group.animatedChildren(moved,rotateds,scaleds);
topToBottom(30)(animated,animatedChildren);

drawObjects(animatedChildren);
% end children of animated
% children of scaled
Class.Blinker("Blinker")
		( "+time_dead:float ",
		"+time_alive:float ",
		"+next_switch:float ",
		"+state:stan " )
		( "+UpdateAnimation():bool ",
		"+drawIt():bool ",
		"+canEnter(Vector3D &zmiana):bool ",
		"+canFall():bool ");
Class.stan("stan")
		("+visible",
		"+shrinking",
		"+hidden",
		"+growing")();
classStereotypes.stan("<<enumerate>>");

topToBottom(30)(scaleds,Blinker);
leftToRight(20)(Blinker, stan);

drawObjects(Blinker,stan);
% end children of scaled
% children of moved
Class.Player("Player") ()
		("+pushObj(obj:moved)");
Class.Door("Door")
		( "+openPos:Vector3D ",
		"+closedPos:Vector3D ")
		( "+respondON():bool ",
		"+respondOFF():bool ",
		"+drawIt():bool ",
		"+canEnter(Vector3D &zmiana):bool ");
Class.Box("Box") ()
	("+drawIt():bool ",
	"+canEnter(pos: Vector3D):bool ",
	"+playerEnters(pos: Vector3D):void ");
Box.info.foreColor := (1,1,.5);

Class.Trigger("Trigger") () ();
Trigger.info.foreColor := (.9,.7,.7);

leftToRight.top(20)(Player,Door,Box);
leftToRight.top(30)(Box,Trigger);

Group.movedChildren(Player,Door,Box, Trigger);
topToBottom.left(200)(moved,movedChildren);

drawObjects(movedChildren);
	

% end children of moved
% children of rotated
Class.Gem("Gem")
	()
	("+drawIt():bool ",
	"+playerEnters(pos: Vector3D):void ");
Gem.info.foreColor := (1,1,.5);

topToBottom(20)(rotateds, Gem);
drawObjects(Gem);
% end children of rotated

Class.Responder("Responder") ()
	("+respondON(): bool",
	"+respondOFF(): bool");
Responder.info.foreColor := (.9,.7,.7);
topToBottom(30)(Responder,Switch);

drawObjects(Responder);


% begin links
clink(associationUni)(Vector, game_obj);

clink(inheritance)(game_obj, animated);
clink(inheritance)(game_obj, Wall);
clink(inheritance)(game_obj, Floor);
clink(inheritance)(game_obj, EmptySpace);
clink(inheritance)(game_obj, Switch);

clink(inheritance)(animated, moved);
clink(inheritance)(animated, rotateds);
clink(inheritance)(animated, scaleds);

clink(inheritance)(moved, Player);
clink(inheritance)(moved, Door);
clink(inheritance)(moved, Box);

clink(inheritance)(rotateds, Gem);

clink(inheritance)(scaleds, Blinker);
clink(associationUni)(stan, Blinker);

clink(inheritance)(Trigger, Box);
clink(inheritance)(Responder, Switch);

endfig;
end;
