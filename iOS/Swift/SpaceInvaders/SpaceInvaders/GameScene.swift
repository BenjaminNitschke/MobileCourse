import SpriteKit

class GameScene: SKScene {
	override func didMoveToView(view: SKView) {
		createBackground()
		createShip()
		createEnemies()
	}
	
	func createBackground() {
		let background = SKSpriteNode(imageNamed:"Background")
		screenWidth = CGRectGetWidth(self.frame)
		screenHeight = CGRectGetHeight(self.frame)
		background.position = CGPoint(x: screenWidth / 2, y: screenHeight / 2)
		background.size = CGSize(width: screenWidth, height: screenHeight)
		self.addChild(background)
	}
	
	var screenWidth : CGFloat = 0
	var screenHeight : CGFloat = 0
	
	func createShip() {
		ship = SKSpriteNode(imageNamed:"Ship")
		ship!.position = CGPoint(x: screenWidth / 2, y: screenHeight * 0.2)
		self.addChild(ship!)
	}
	
	var ship : SKSpriteNode?
	
	func createEnemies() {
		for var y : CGFloat = 0.65; y < 0.8; y+=0.1 {
			for var x : CGFloat = 0.1; x < 0.95; x+=0.1 {
				let	enemy = SKSpriteNode(imageNamed:"Enemy")
				enemy.position = CGPoint(x: x * screenWidth, y: y * screenHeight)
				self.addChild(enemy)
				enemies.append(enemy)
			}
		}
	}
	
	var enemies : [SKSpriteNode] = []
	
	override func touchesMoved(touches: Set<NSObject>, withEvent event: UIEvent) {
		for touch in (touches as! Set<UITouch>) {
		   ship!.position.x = touch.locationInNode(self).x
		}
	}
	
	override func update(currentTime: CFTimeInterval) {
		controlShip(currentTime)
		moveEnemies(currentTime)
	}
	
	func controlShip(currentTime: CFTimeInterval) {
		if (lastTimeShot < currentTime - 0.33) {
			fireMissile(currentTime)
		}
		handleMissiles()
	}
	
	var lastTimeShot : CFTimeInterval = 0.0
	
	func fireMissile(currentTime: CFTimeInterval) {
		lastTimeShot = currentTime
		let	missile = SKSpriteNode(imageNamed:"Missile")
		missile.position = ship!.position
		self.addChild(missile)
		activeMissiles.append(missile)
	}
	
	var activeMissiles : [SKSpriteNode] = []
	
	func handleMissiles() {
		var missilesToRemove : [Int] = []
		var index = 0
		for missile in activeMissiles {
			missile.position = CGPoint(
				x: missile.position.x,
				y: missile.position.y + 0.02 * screenHeight)
			if (missile.position.y > 1.0 * screenHeight || collidingWithEnemy(missile)) {
				missilesToRemove.append(index)
			}
			index++
		}
		for missileIndex in missilesToRemove {
			activeMissiles[missileIndex].removeFromParent()
			activeMissiles.removeAtIndex(missileIndex)
		}
	}
	
	func collidingWithEnemy(missile: SKSpriteNode) -> Bool {
		for enemy in enemies {
			var distanceX = missile.position.x - enemy.position.x
			var distanceY = missile.position.y - enemy.position.y
			var distance = sqrt(distanceX * distanceX + distanceY * distanceY)
			if (distance < 0.025 * screenWidth) {
				enemy.position = CGPoint(x: 0.0, y: -1000.0)
				return true
			}
		}
		return false
	}
	
	func moveEnemies(currentTime: CFTimeInterval) {
		if (enemiesMovingRight) {
			xOffset++
		}
		else {
			xOffset--
		}
		if (xOffset > 75 || xOffset < -75) {
			enemiesMovingRight = !enemiesMovingRight
		}
		
		var index = 0
		for var y : CGFloat = 0.65; y < 0.8; y+=0.1 {
			for var x : CGFloat = 0.1; x < 0.95; x+=0.1 {
				if (enemies[index].position.y > 0) {
					enemies[index].position = CGPoint(x: x * screenWidth + xOffset, y: y * screenHeight)
				}
				index++
			}
		}
	}
	var xOffset : CGFloat = 0
	var enemiesMovingRight = false
}
