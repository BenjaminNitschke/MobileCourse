//
//  Shader.fsh
//  TestCppCode
//
//  Created by Benjamin Nitschke on 20/06/15.
//  Copyright (c) 2015 DeltaEngine. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
