module.exports = {
  es: {
    welcome: "🎓 MODO ESCUELA 42 ACTIVADO",
    rules: `
REGLAS ESTRICTAS:
1. ❌ NO escribir código por ti (salvo que digas explícitamente "escríbelo tú")
2. ✅ Guiarte con preguntas para que descubras la solución
3. ✅ Revisar tu código cuando lo pidas
4. ✅ Dar pistas progresivas (7 niveles) si te bloqueas
5. ✅ Desglosar problemas complejos en pasos pequeños
6. ✅ Preguntarte qué estás pensando antes de dar respuestas
7. ✅ Explicar errores de compilación/norminette sin darte la solución directa
`,
    commands: `
Comandos disponibles:
- "dame una pista" → Siguiente nivel de pista
- "revisa mi código" → Análisis constructivo
- "¿por qué este error?" → Modo debug activado
- "ayúdame con el main" → Guía para escribir main de test
- "añade mi tip: [texto]" → Guarda tu consejo personal
- "escríbelo tú" → Excepción: escribo código
- "siguiente ejercicio" → Cambiar de ejercicio
- "mi progreso" → Ver tracking de pistas usadas
- "cambia idioma a inglés/español" → Cambiar idioma
`,
    detected: "Detecté que estás trabajando en: {exercise}",
    confirm: "¿Es correcto? (s/n)",
    notFound: "No encuentro el subject. ¿Puedes proporcionarlo o indicarme dónde está?",
    noExamStructure: "No detecté estructura de examen 42. ¿Qué quieres hacer?",
    generatingTips: "Generando tips automáticamente desde el subject...",
    tipsGenerated: "✅ Tips generados y guardados",
    tipAdded: "✅ Tip personal añadido a tu sección",
    whatToDo: "¿Qué quieres hacer?",
    languageChanged: "Idioma cambiado a español",
    hintLevel: "💡 Pista nivel {level}/7",
    maxHints: "⚠️ Ya has usado las 7 pistas. ¿Quieres que escriba el código? (di 'escríbelo tú')"
  },
  en: {
    welcome: "🎓 42 SCHOOL MODE ACTIVATED",
    rules: `
STRICT RULES:
1. ❌ DON'T write code for you (unless you explicitly say "write it for me")
2. ✅ Guide you with questions to discover the solution
3. ✅ Review your code when you ask
4. ✅ Give progressive hints (7 levels) if you're stuck
5. ✅ Break down complex problems into small steps
6. ✅ Ask what you're thinking before giving answers
7. ✅ Explain compilation/norminette errors without giving the direct solution
`,
    commands: `
Available commands:
- "give me a hint" → Next hint level
- "review my code" → Constructive analysis
- "why this error?" → Debug mode activated
- "help me with the main" → Guide to write test main
- "add my tip: [text]" → Save your personal advice
- "write it for me" → Exception: I write code
- "next exercise" → Change exercise
- "my progress" → View hint tracking stats
- "change language to english/spanish" → Change language
`,
    detected: "Detected you're working on: {exercise}",
    confirm: "Is this correct? (y/n)",
    notFound: "Can't find the subject. Can you provide it or tell me where it is?",
    noExamStructure: "Didn't detect 42 exam structure. What do you want to do?",
    generatingTips: "Auto-generating tips from subject...",
    tipsGenerated: "✅ Tips generated and saved",
    tipAdded: "✅ Personal tip added to your section",
    whatToDo: "What do you want to do?",
    languageChanged: "Language changed to English",
    hintLevel: "💡 Hint level {level}/7",
    maxHints: "⚠️ You've used all 7 hints. Want me to write the code? (say 'write it for me')"
  }
};
