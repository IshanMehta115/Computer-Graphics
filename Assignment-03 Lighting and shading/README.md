# Assignment-03: Lighting and Shading

## Project Overview

This assignment, titled "Lighting and Shading," is a part of our course curriculum in computer graphics. The primary focus of this project is to understand and implement various lighting and shading techniques to enhance the visual realism of 3D scenes.

## Assignment Details

- Detailed assignment requirements and guidelines are provided in [assignment03.pdf](https://github.com/IshanMehta115/Computer-Graphics/blob/main/Assignment-03%20Lighting%20and%20shading/Assignment03.pdf).
- My comprehensive report, including my implementation details and findings, can be found in [report.pdf](https://github.com/IshanMehta115/Computer-Graphics/blob/main/Assignment-03%20Lighting%20and%20shading/Report.pdf).

## Key Features and Objectives

In this assignment, I explored several key components related to lighting and shading in computer graphics:

1. **Normal Vectors for Shading**
   - I implemented the computation of normal vectors at each vertex. These normals are crucial for lighting and shading calculations.

2. **Lighting Models**
   - I applied various lighting models, including ambient, specular, and diffuse lighting, to improve the visual appearance of 3D objects.

3. **Lighting Formulas**
   - I incorporated the following lighting formulas with descriptions:
     - Ambient Lighting: Ambient light represents the background or global illumination. It's a constant light source that adds uniform brightness to all surfaces.
       - Formula: `Ambient = AmbientColor * AmbientIntensity`
     - Specular Lighting: Specular highlights are bright spots that appear on shiny surfaces when illuminated. This component simulates the reflection of light.
       - Formula: `Specular = SpecularColor * SpecularIntensity * (R.V)^Shininess`
     - Diffuse Lighting: Diffuse lighting accounts for the direction of incoming light and the surface's orientation.
       - Formula: `Diffuse = DiffuseColor * DiffuseIntensity * (N.L)`

5. **Shader Modifications**
   - I made necessary changes to the vertex shader and fragment shader to incorporate the lighting calculations.

6. **Visualizing Normals**
   - As an additional feature, I set the color of each vertex based on the normalized value of its associated normal vector. This visualization helps understand the orientation of normals.

