# 贡献指南

感谢您考虑为本项目做出贡献！🎉

## 如何贡献

### 报告 Bug

如果您发现了 bug，请创建一个 issue 并包含以下信息：

- **Bug 描述**：清晰简洁地描述问题
- **复现步骤**：详细说明如何复现这个问题
- **期望行为**：描述您期望发生什么
- **实际行为**：描述实际发生了什么
- **环境信息**：操作系统、浏览器版本、Node.js 版本等
- **截图**：如果适用，添加截图帮助说明问题

### 建议新功能

我们欢迎新功能建议！请创建一个 issue 并包含：

- **功能描述**：清晰描述您希望添加的功能
- **使用场景**：说明这个功能的使用场景
- **可能的实现方式**：如果有想法，可以简单描述

### 提交代码

1. **Fork 仓库**
   ```bash
   # 点击 GitHub 页面右上角的 Fork 按钮
   ```

2. **克隆您的 Fork**
   ```bash
   git clone https://github.com/YOUR-USERNAME/deep-learning-reproductions.git
   cd deep-learning-reproductions
   ```

3. **创建分支**
   ```bash
   git checkout -b feature/your-feature-name
   # 或
   git checkout -b fix/your-bug-fix
   ```

4. **安装依赖**
   ```bash
   npm install
   ```

5. **进行修改**
   - 遵循项目的代码风格
   - 添加必要的注释
   - 确保代码可以正常运行

6. **测试您的修改**
   ```bash
   npm start  # 启动开发服务器
   npm test   # 运行测试
   ```

7. **提交更改**
   ```bash
   git add .
   git commit -m "feat: 添加XXX功能"
   # 或
   git commit -m "fix: 修复XXX问题"
   ```

   提交信息格式：
   - `feat:` 新功能
   - `fix:` Bug 修复
   - `docs:` 文档更新
   - `style:` 代码格式调整
   - `refactor:` 代码重构
   - `test:` 测试相关
   - `chore:` 构建/工具链更新

8. **推送到您的 Fork**
   ```bash
   git push origin feature/your-feature-name
   ```

9. **创建 Pull Request**
   - 访问您的 Fork 页面
   - 点击 "New Pull Request"
   - 填写 PR 描述，说明您的更改
   - 等待审核

## 代码规范

### JavaScript/React 规范

- 使用 ES6+ 语法
- 函数组件优先于类组件
- 使用 Hooks 管理状态
- 组件名使用 PascalCase
- 变量和函数名使用 camelCase
- 适当添加注释

### 代码示例

```jsx
// ✅ 好的写法
const MyComponent = ({ title, description }) => {
  const [isActive, setIsActive] = useState(false);

  const handleClick = () => {
    setIsActive(!isActive);
  };

  return (
    <div className="component">
      <h2>{title}</h2>
      <p>{description}</p>
      <button onClick={handleClick}>Toggle</button>
    </div>
  );
};

// ❌ 避免的写法
function mycomponent(props) {
  var active = false;
  return <div><h2>{props.title}</h2></div>;
}
```

## 添加新模型

如果您想添加新的深度学习模型实现：

1. 在 `modelConfigs.js` 中添加新模型配置：

```javascript
export const modelConfigs = {
  // ... 现有模型
  
  your_model: {
    name: 'Your Model (2024)',
    paper: 'Paper Title',
    code: `# PyTorch 实现代码
import torch
import torch.nn as nn

class YourModel(nn.Module):
    def __init__(self):
        super().__init__()
        # 模型定义
    
    def forward(self, x):
        # 前向传播
        return x`,
    status: '✓ 已复现'
  }
};
```

2. 确保代码：
   - 完整可运行
   - 包含必要的注释
   - 提供使用示例
   - 符合 PyTorch 最佳实践

## 文档贡献

文档改进同样重要！您可以：

- 修正拼写或语法错误
- 改进说明的清晰度
- 添加示例和教程
- 翻译文档到其他语言

## 问题讨论

在开始大型更改之前：

1. 先创建一个 issue 讨论您的想法
2. 等待维护者的反馈
3. 达成共识后再开始实现

## 行为准则

- 保持友好和尊重
- 欢迎不同观点
- 专注于对项目最有利的方案
- 展现同理心

## 审核流程

提交 PR 后：

1. **自动检查**：CI/CD 会自动运行测试
2. **代码审核**：维护者会审核您的代码
3. **反馈与修改**：根据反馈进行必要的修改
4. **合并**：通过审核后会被合并到主分支

## 获取帮助

如果您有任何问题：

- 查看现有的 [Issues](https://github.com/yourusername/deep-learning-reproductions/issues)
- 创建新的 issue 提问
- 在 PR 中评论询问

## 致谢

每一个贡献者都会被添加到项目的致谢列表中！

---

再次感谢您的贡献！🙏

