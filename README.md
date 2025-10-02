# 🧠 深度学习论文复现集合

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![React](https://img.shields.io/badge/React-18.0+-blue.svg)](https://reactjs.org/)
[![PyTorch](https://img.shields.io/badge/PyTorch-Compatible-red.svg)](https://pytorch.org/)

一个精美的深度学习经典论文复现项目，使用 React 构建的交互式展示界面，包含 8 个经典模型的完整 PyTorch 实现。

## ✨ 特性

- 🎯 **8个经典模型** - ResNet, Transformer, GAN, VAE, U-Net, YOLO, ViT
- 💻 **完整实现** - 包含模型定义、前向传播和使用示例
- 🎨 **现代UI** - 使用 Tailwind CSS 构建的精美界面
- 📦 **即用即部署** - 代码可直接复制运行
- 🔍 **详细注释** - 每个模型都包含论文引用和说明

## 📚 已复现模型

| 模型 | 年份 | 论文 | 状态 |
|------|------|------|------|
| ResNet | 2015 | Deep Residual Learning for Image Recognition | ✓ 已复现 |
| Transformer | 2017 | Attention Is All You Need | ✓ 已复现 |
| GAN | 2014 | Generative Adversarial Networks | ✓ 已复现 |
| VAE | 2013 | Auto-Encoding Variational Bayes | ✓ 已复现 |
| U-Net | 2015 | U-Net: Convolutional Networks for Biomedical Image Segmentation | ✓ 已复现 |
| YOLO | 2015 | You Only Look Once: Unified, Real-Time Object Detection | ✓ 已复现 |
| ViT | 2020 | An Image is Worth 16x16 Words | ✓ 已复现 |

## 🚀 快速开始

### 安装依赖

```bash
npm install
```

### 运行项目

```bash
npm start
```

项目将在 `http://localhost:3000` 启动。

### 构建生产版本

```bash
npm run build
```

## 📁 项目结构

```
tcp/
├── DeepLearningReproductions.jsx  # 主组件
├── modelConfigs.js                # 模型配置文件
├── package.json                   # 项目依赖
├── README.md                      # 项目文档
└── .gitignore                     # Git 忽略文件
```

## 💡 使用方法

### 在 React 应用中使用

```jsx
import DeepLearningReproductions from './DeepLearningReproductions';

function App() {
  return (
    <div className="App">
      <DeepLearningReproductions />
    </div>
  );
}
```

### 使用 PyTorch 代码

1. 选择你想要的模型
2. 复制代码到你的 Python 文件
3. 安装 PyTorch: `pip install torch torchvision`
4. 直接运行代码

## 🛠️ 技术栈

- **前端框架**: React 18+
- **样式**: Tailwind CSS
- **图标**: Lucide React
- **深度学习框架**: PyTorch

## 📖 模型详解

### ResNet (残差网络)
通过引入跳跃连接解决了深度神经网络的梯度消失问题，使得训练超过100层的网络成为可能。

### Transformer
革命性的注意力机制架构，成为现代 NLP 和多模态模型的基础。

### GAN (生成对抗网络)
包含生成器和判别器的对抗训练框架，可以生成高质量的图像。

### VAE (变分自编码器)
概率生成模型，学习数据的潜在表示，可用于生成和重构。

### U-Net
医学图像分割的经典架构，采用编码器-解码器结构和跳跃连接。

### YOLO
实时目标检测算法，将检测问题转化为回归问题。

### ViT (视觉Transformer)
将 Transformer 架构应用到计算机视觉，通过图像块嵌入处理图像。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

### 贡献指南

1. Fork 本仓库
2. 创建你的特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交你的改动 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启一个 Pull Request

## 📝 待办事项

- [ ] 添加更多经典模型 (BERT, GPT, Diffusion Models)
- [ ] 提供模型训练脚本
- [ ] 添加模型性能对比
- [ ] 支持代码高亮
- [ ] 添加模型可视化

## 📄 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 🙏 致谢

- 感谢所有原始论文的作者
- 感谢 PyTorch 团队提供的优秀框架
- 感谢开源社区的贡献

## 📮 联系方式

如有问题或建议，请通过以下方式联系：

- 提交 GitHub Issue
- 发送邮件到：[your-email@example.com]

---

⭐ 如果这个项目对你有帮助，请给个 Star！

