---
presentation:
  theme: serif.css
  mouseWheel: true
  width: 1000
  height: 900
---

<!-- slide -->

### 从零开始实现一个软渲染器
1953910 李林洲

<!-- slide -->

## 依赖

位于`include`和`lib`文件夹下。

- GLFW  c++图形化界面库
- OpenGL  仅使用了`glViewport`与`glDrawPixels`两个函数
- glm  矩阵运算数学库
- stb_image.h  图片文件加载
- glad  用户加载OpenGL函数

<!-- slide -->

## 类图

<img src="img/classDiag.png">

<!-- slide -->

## 渲染流程

<img src="img/pipeline.png" width=25%>

<!-- slide -->

### drawMesh

<img src="img/drawMesh.png" width=80%>

<!-- slide -->

#### 顶点着色器

- 计算世界坐标(World Position)
- 计算窗口坐标(Window Position)

Window Position = Project Matrix * View Matrix * Model Matrix * Position (MVP矩阵)

透视投影矩阵：
$$
\left(
\begin{array}{l}
\frac{n}{r} & 0 & 0 & 0 \\
0 & \frac{n}{t} & 0 & 0 \\
0 & 0 & \frac{-(f+n)}{f-n} & \frac{-2fn}{f-n} \\
0 & 0 & -1 & 0 \\
\end{array}
\right)
$$

<!-- slide -->

#### 剔除

背面剔除：相机正方向与三角面不一致时剔除

视锥剔除：计算当前摄像机对应的视锥平面(上下左右远近)，三个点都不在视锥中的平面剔除

<img src="img\视锥剔除.png" width=30%>

<!-- slide -->

### drawTriangle

#### 扫描线算法

<img src="img\scanline.png" width=50%>

<!-- slide -->

#### 边界函数算法

<img src="img\edgeFunction.png" width=50%>

<!-- slide -->

分块优化：

1. 完全在三角形内
2. 部分在三角形内
3. 完全在三角形外

<img src="img\分块优化.png" width=40%>

<!-- slide -->

重心坐标系：

- 重心坐标系下，x,y,z坐标均大于零则此点在三角形中
- 方便插值

<img src="img/重心坐标.png" width=30%>

结果：单线程运行，边界函数算法反而比扫描线算法更慢

<!-- slide -->

### fragmentShader

#### 纹理映射

仿射纹理映射

<img src="img\仿射纹理映射.png" width=40%>

<!-- slide -->

透视纹理映射

<img src="img\透视纹理映射.png" width=40%>

<!-- slide -->

#### 光照

漫反射：从各处看到的反射光线都是一致的，漫反射的强度只跟物体表面与光线夹角有关。

```c++
float diff = dot(normal, -lightDir);
vec3 result = diff * lightColor * vertexColor * Intensity;
```

镜面反射：强度取决于反射光线与观察方向

```c++
vec3 reflectDir = normalize(lightDir - 2 * dot(normal, lightDir) * normal); // 反射光方向
vec3 viewDir = normalize(cameraPos - worldPos); // 观察方向
float spec = pow(dot(reflectDir, viewDir), gloss);
vec3 result = spec * lightColor * Intensity
```
<!-- slide -->

## 渲染效果
<img src="img\result.png" width=400>

<!-- slide -->

# Thanks
