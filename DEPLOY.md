# 部署指南

本指南将帮助您将深度学习论文复现集合部署到各种平台。

## 📦 构建生产版本

在部署之前，首先构建生产版本：

```bash
npm run build
```

这将创建一个优化的生产构建在 `build/` 目录中。

## 🚀 部署选项

### 1. GitHub Pages

最简单的免费部署方式。

#### 步骤：

1. **安装 gh-pages**
```bash
npm install --save-dev gh-pages
```

2. **在 package.json 中添加部署脚本**
```json
{
  "homepage": "https://yourusername.github.io/deep-learning-reproductions",
  "scripts": {
    "predeploy": "npm run build",
    "deploy": "gh-pages -d build"
  }
}
```

3. **部署**
```bash
npm run deploy
```

4. **启用 GitHub Pages**
   - 访问仓库的 Settings → Pages
   - Source 选择 `gh-pages` 分支
   - 保存

### 2. Vercel

零配置部署，速度极快。

#### 步骤：

1. **安装 Vercel CLI**
```bash
npm install -g vercel
```

2. **登录并部署**
```bash
vercel login
vercel
```

3. **或者通过 GitHub 集成**
   - 访问 [vercel.com](https://vercel.com)
   - 导入您的 GitHub 仓库
   - 自动部署

### 3. Netlify

提供持续部署和免费 HTTPS。

#### 方式一：拖放部署

1. 运行 `npm run build`
2. 访问 [netlify.com](https://www.netlify.com)
3. 将 `build/` 文件夹拖放到部署区域

#### 方式二：Git 集成

1. 访问 [netlify.com](https://www.netlify.com)
2. 点击 "New site from Git"
3. 连接您的 GitHub 仓库
4. 配置构建设置：
   - Build command: `npm run build`
   - Publish directory: `build`

### 4. AWS S3 + CloudFront

适合需要自定义配置的企业应用。

#### 步骤：

1. **创建 S3 存储桶**
```bash
aws s3 mb s3://your-bucket-name
```

2. **上传构建文件**
```bash
aws s3 sync build/ s3://your-bucket-name
```

3. **配置静态网站托管**
```bash
aws s3 website s3://your-bucket-name \
  --index-document index.html \
  --error-document index.html
```

4. **配置 CloudFront**（可选，用于 CDN 加速）

### 5. Docker 部署

使用容器化部署。

#### Dockerfile

```dockerfile
# 构建阶段
FROM node:18-alpine as build
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build

# 生产阶段
FROM nginx:alpine
COPY --from=build /app/build /usr/share/nginx/html
COPY nginx.conf /etc/nginx/conf.d/default.conf
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
```

#### nginx.conf

```nginx
server {
    listen 80;
    server_name localhost;
    root /usr/share/nginx/html;
    index index.html;

    location / {
        try_files $uri $uri/ /index.html;
    }

    # 缓存静态资源
    location ~* \.(js|css|png|jpg|jpeg|gif|ico|svg)$ {
        expires 1y;
        add_header Cache-Control "public, immutable";
    }
}
```

#### 构建和运行

```bash
# 构建镜像
docker build -t deep-learning-reproductions .

# 运行容器
docker run -p 80:80 deep-learning-reproductions
```

### 6. Heroku

传统 PaaS 平台。

#### 步骤：

1. **安装 Heroku CLI**
```bash
npm install -g heroku
```

2. **登录**
```bash
heroku login
```

3. **创建应用**
```bash
heroku create your-app-name
```

4. **添加 buildpack**
```bash
heroku buildpacks:set heroku/nodejs
```

5. **部署**
```bash
git push heroku main
```

## 🔧 环境变量配置

如果您的应用需要环境变量：

### 创建 `.env` 文件

```env
REACT_APP_API_URL=https://api.example.com
REACT_APP_ANALYTICS_ID=your-analytics-id
```

### 在各平台配置

**Vercel:**
```bash
vercel env add REACT_APP_API_URL
```

**Netlify:**
Site settings → Build & deploy → Environment

**GitHub Pages:**
使用 GitHub Secrets 在构建时注入

## 🎯 性能优化

### 代码分割

已在项目中默认启用，通过 React.lazy() 实现。

### 压缩资源

```bash
npm install --save-dev compression-webpack-plugin
```

### 使用 CDN

将静态资源托管到 CDN（如 Cloudflare、AWS CloudFront）。

### 启用 Gzip/Brotli 压缩

大多数现代托管平台已自动启用。

## 📊 监控和分析

### Google Analytics

在 `public/index.html` 中添加：

```html
<!-- Google Analytics -->
<script async src="https://www.googletagmanager.com/gtag/js?id=GA_MEASUREMENT_ID"></script>
<script>
  window.dataLayer = window.dataLayer || [];
  function gtag(){dataLayer.push(arguments);}
  gtag('js', new Date());
  gtag('config', 'GA_MEASUREMENT_ID');
</script>
```

### Sentry 错误监控

```bash
npm install @sentry/react
```

## 🔒 安全配置

### HTTPS

确保您的部署平台启用了 HTTPS（大多数平台默认启用）。

### CSP 头部

添加内容安全策略：

```html
<meta http-equiv="Content-Security-Policy" 
      content="default-src 'self'; script-src 'self' 'unsafe-inline';">
```

### 环境变量保护

永远不要将敏感信息提交到 Git，使用环境变量。

## 🧪 部署前检查清单

- [ ] 运行 `npm run build` 确认构建成功
- [ ] 测试生产构建 `npx serve -s build`
- [ ] 检查所有链接是否正常工作
- [ ] 验证响应式设计在各设备上的表现
- [ ] 测试不同浏览器的兼容性
- [ ] 确认环境变量正确配置
- [ ] 检查控制台是否有错误
- [ ] 运行性能测试（Lighthouse）
- [ ] 确认 SEO 元标签正确设置

## 🔄 持续部署 (CI/CD)

### GitHub Actions

创建 `.github/workflows/deploy.yml`：

```yaml
name: Deploy

on:
  push:
    branches: [ main ]

jobs:
  build-and-deploy:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Setup Node.js
      uses: actions/setup-node@v3
      with:
        node-version: '18'
        
    - name: Install dependencies
      run: npm ci
      
    - name: Build
      run: npm run build
      
    - name: Deploy to GitHub Pages
      uses: peaceiris/actions-gh-pages@v3
      with:
        github_token: ${{ secrets.GITHUB_TOKEN }}
        publish_dir: ./build
```

## 📱 移动端优化

确保添加 viewport 和移动端元标签：

```html
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta name="mobile-web-app-capable" content="yes" />
<meta name="apple-mobile-web-app-capable" content="yes" />
```

## 🌐 自定义域名

### GitHub Pages

1. 在仓库根目录创建 `CNAME` 文件
2. 添加您的域名，如 `example.com`
3. 在域名提供商配置 DNS：
   ```
   A    @    185.199.108.153
   A    @    185.199.109.153
   A    @    185.199.110.153
   A    @    185.199.111.153
   ```

### Vercel/Netlify

在平台设置中直接添加自定义域名。

## 🆘 故障排查

### 页面空白

- 检查浏览器控制台错误
- 确认 `homepage` 字段配置正确
- 检查路由配置

### 样式未加载

- 确认 Tailwind CSS 正确配置
- 检查 CSS 文件路径

### 404 错误

- 配置服务器重定向到 `index.html`
- 使用 HashRouter 替代 BrowserRouter

---

需要帮助？请查看各平台的官方文档或提交 Issue。

