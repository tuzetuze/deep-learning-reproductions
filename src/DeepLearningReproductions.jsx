import React, { useState } from 'react';
import { Code, BookOpen, Cpu, Zap } from 'lucide-react';
import { modelConfigs } from './modelConfigs';

const DeepLearningReproductions = () => {
  const [selectedModel, setSelectedModel] = useState('resnet');

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-900 via-purple-900 to-slate-900 p-6">
      <div className="max-w-7xl mx-auto">
        {/* Header Section */}
        <header className="text-center mb-8">
          <h1 className="text-4xl font-bold text-white mb-2 flex items-center justify-center gap-3">
            <Cpu className="text-purple-400" size={40} />
            深度学习论文复现集合
          </h1>
          <p className="text-purple-300">经典论文的完整 PyTorch 实现</p>
        </header>

        {/* Model Selection Grid */}
        <nav className="grid grid-cols-2 md:grid-cols-4 gap-3 mb-6" role="navigation" aria-label="模型选择">
          {Object.entries(modelConfigs).map(([key, model]) => (
            <button
              key={key}
              onClick={() => setSelectedModel(key)}
              className={`p-4 rounded-lg transition-all transform hover:scale-105 focus:outline-none focus:ring-2 focus:ring-purple-500 ${
                selectedModel === key
                  ? 'bg-purple-600 text-white shadow-xl'
                  : 'bg-slate-800 text-slate-300 hover:bg-slate-700'
              }`}
              aria-pressed={selectedModel === key}
            >
              <div className="font-semibold">{model.name}</div>
              <div className="text-xs mt-1 opacity-80">{model.status}</div>
            </button>
          ))}
        </nav>

        {/* Model Details Card */}
        <article className="bg-slate-800 rounded-xl shadow-2xl overflow-hidden">
          {/* Card Header */}
          <header className="bg-gradient-to-r from-purple-600 to-pink-600 p-4 flex items-center justify-between">
            <div className="flex items-center gap-3">
              <BookOpen className="text-white" size={24} />
              <div>
                <h2 className="text-xl font-bold text-white">
                  {modelConfigs[selectedModel].name}
                </h2>
                <p className="text-sm text-purple-100">
                  {modelConfigs[selectedModel].paper}
                </p>
              </div>
            </div>
            <Zap className="text-yellow-300" size={28} />
          </header>

          {/* Card Body */}
          <div className="p-6">
            <div className="flex items-center gap-2 mb-3">
              <Code className="text-purple-400" size={20} />
              <h3 className="text-lg font-semibold text-white">完整实现代码</h3>
            </div>
            
            <div className="bg-slate-900 rounded-lg p-4 overflow-x-auto">
              <pre className="text-sm text-green-400 font-mono">
                <code>{modelConfigs[selectedModel].code}</code>
              </pre>
            </div>

            <div className="mt-4 p-4 bg-purple-900 bg-opacity-30 rounded-lg border border-purple-500">
              <p className="text-purple-200 text-sm">
                <strong>提示：</strong> 代码已完整实现，可直接复制运行。包含模型定义、前向传播和使用示例。
              </p>
            </div>
          </div>
        </article>

        {/* Feature Cards */}
        <footer className="mt-6 grid grid-cols-1 md:grid-cols-3 gap-4">
          <div className="bg-slate-800 p-4 rounded-lg">
            <h3 className="text-white font-semibold mb-2">✓ 已复现模型</h3>
            <p className="text-purple-300 text-sm">8个经典深度学习模型</p>
          </div>
          <div className="bg-slate-800 p-4 rounded-lg">
            <h3 className="text-white font-semibold mb-2">📦 框架支持</h3>
            <p className="text-purple-300 text-sm">PyTorch 完整实现</p>
          </div>
          <div className="bg-slate-800 p-4 rounded-lg">
            <h3 className="text-white font-semibold mb-2">🚀 即用即部署</h3>
            <p className="text-purple-300 text-sm">包含训练和推理代码</p>
          </div>
        </footer>
      </div>
    </div>
  );
};

export default DeepLearningReproductions;

