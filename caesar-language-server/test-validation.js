// Quick test of our validation patterns
const text = `def test():
    if x = 5:  # Assignment instead of equality
        print("unclosed string
    return x

undefined_var++  # Undefined variable`;

const lines = text.split('\n');

console.log('🔍 Testing validation patterns:');

for (let i = 0; i < lines.length; i++) {
  const line = lines[i];
  console.log(`Line ${i}: "${line}"`);
  
  // Check unclosed strings
  if (line.includes('"') && (line.split('"').length - 1) % 2 !== 0) {
    console.log(`  ❌ Unclosed string detected!`);
  }
  
  // Check assignment in if
  const assignmentInIf = /if\s+.*\s*=\s*[^=]/.test(line);
  if (assignmentInIf) {
    console.log(`  ⚠️ Assignment in if statement detected!`);
  }
  
  // Check undefined variables
  const undefinedVar = /^(\s*)([a-zA-Z_][a-zA-Z0-9_]*)\s*\+\+/.exec(line);
  if (undefinedVar && !text.includes(`${undefinedVar[2]} =`)) {
    console.log(`  ⚠️ Undefined variable '${undefinedVar[2]}' detected!`);
  }
}