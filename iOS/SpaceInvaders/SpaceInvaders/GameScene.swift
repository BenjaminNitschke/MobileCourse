import SpriteKit

class GameScene: SKScene {
	override func didMoveToView(view: SKView) {
		let background = SKSpriteNode(imageNamed:"Background")
		let width = CGRectGetWidth(self.frame)
		let height = CGRectGetHeight(self.frame)
		background.position = CGPoint(x: width/2, y: height/2)
		background.size = CGSize(width: width, height: height)
		self.addChild(background)
		ship = SKSpriteNode(imageNamed:"Ship")
		ship?.position = CGPoint(x: width/2, y: height*0.2) // bottom up
		self.addChild(ship!)
	}
	
	var ship: SKSpriteNode?
	
	override func touchesMoved(touches: Set<NSObject>, withEvent event: UIEvent) {
		for touch in (touches as! Set<UITouch>) {
			ship?.position.x = touch.locationInNode(self).x
		}
	}
	
	override func update(currentTime: CFTimeInterval) {}
}