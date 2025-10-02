# 📤 GitHub 上传指南

## 步骤 1: 在 GitHub 上创建新仓库

1. 访问 [GitHub](https://github.com) 并登录你的账户
2. 点击右上角的 `+` 号，选择 "New repository"
3. 填写仓库信息：
   - **Repository name**: `deep-learning-reproductions` （或你喜欢的名字）
   - **Description**: `经典深度学习论文复现集合 - 包含8个经典模型的完整PyTorch实现`
   - **Public/Private**: 选择 Public（公开）或 Private（私有）
   - **⚠️ 重要**: 不要勾选 "Add a README file"、"Add .gitignore" 或 "Choose a license"（我们已经有这些文件了）
4. 点击 "Create repository"

## 步骤 2: 推送代码到 GitHub

GitHub 会显示一个页面，包含推送现有仓库的命令。在终端中运行以下命令：

### 如果仓库 URL 是 HTTPS 格式：

```bash
# 添加远程仓库（替换 YOUR_USERNAME 为你的 GitHub 用户名）
git remote add origin https://github.com/YOUR_USERNAME/deep-learning-reproductions.git

# 推送代码到 GitHub
git branch -M main
git push -u origin main
```

### 如果仓库 URL 是 SSH 格式：

```bash
# 添加远程仓库（替换 YOUR_USERNAME 为你的 GitHub 用户名）
git remote add origin git@github.com:YOUR_USERNAME/deep-learning-reproductions.git

# 推送代码到 GitHub
git branch -M main
git push -u origin main
```

## 步骤 3: 验证上传

1. 刷新你的 GitHub 仓库页面
2. 你应该能看到所有文件已经上传
3. README.md 会自动显示在仓库首页

## 🎉 完成！

你的项目现在已经托管在 GitHub 上了！

## 后续步骤（可选）

### 1. 更新 package.json 中的仓库链接

编辑 `package.json` 文件，将第 58 行的链接替换为你的实际 GitHub 仓库地址：

```json
"repository": {
  "type": "git",
  "url": "https://github.com/YOUR_USERNAME/deep-learning-reproductions.git"
}
```

### 2. 设置 GitHub Pages（免费托管网站）

如果你想让别人在线访问你的项目：

1. 在你的 GitHub 仓库页面，点击 "Settings"
2. 在左侧菜单中找到 "Pages"
3. 在 "Source" 下拉菜单中选择 "GitHub Actions"
4. 创建 `.github/workflows/deploy.yml` 文件（参考 DEPLOY.md）
5. 推送更改后，你的网站将在 `https://YOUR_USERNAME.github.io/deep-learning-reproductions/` 可访问

### 3. 更新 README.md 中的个人信息

- 第 29 行：将 `"author": "Your Name"` 改为你的名字
- 第 150 行：添加你的联系邮箱

### 4. 日常更新流程

以后修改代码后，使用以下命令推送更新：

```bash
git add .
git commit -m "描述你的修改"
git push
```

## ⚠️ 注意事项

- 确保不要上传 `node_modules` 文件夹（已在 .gitignore 中排除）
- 不要提交敏感信息（密码、API 密钥等）
- 定期推送代码备份

## 🆘 常见问题

### Q: 提示 "remote origin already exists"？

```bash
# 删除现有的 origin 并重新添加
git remote remove origin
git remote add origin https://github.com/YOUR_USERNAME/deep-learning-reproductions.git
```

### Q: 推送时要求输入用户名和密码？

GitHub 现在使用 Personal Access Token（PAT）代替密码：
1. 访问 https://github.com/settings/tokens
2. 点击 "Generate new token (classic)"
3. 选择权限（至少需要 `repo`）
4. 生成后复制 token（只显示一次！）
5. 在推送时使用 token 作为密码

### Q: 想要使用 SSH 而不是 HTTPS？

参考 GitHub 的 SSH 设置指南：https://docs.github.com/cn/authentication/connecting-to-github-with-ssh

---

如有问题，请参考 [GitHub 官方文档](https://docs.github.com/cn) 或提交 Issue。

