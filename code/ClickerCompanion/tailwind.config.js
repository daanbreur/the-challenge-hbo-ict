/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ['./src/renderer/**/*.{js,jsx,ts,tsx,ejs}'],
  theme: {
    fontSize: {
      sm: '0.750rem',
      base: '1rem',
      lg: '1.125rem',
      xl: '1.333rem',
      '2xl': '1.777rem',
      '3xl': '2.369rem',
      '4xl': '3.158rem',
      '5xl': '4.210rem',
    },
    fontFamily: {
      heading: 'Museo',
      body: 'Museo',
    },
    fontWeight: {
      normal: '400',
      semibold: '600',
      bold: '700',
    },
    colors: {
      text: 'var(--text)',
      black_text: 'var(--black-text)',
      gray_text: 'var(--gray-text)',
      background: 'var(--background)',
      medium_background: 'var(--medium-background)',
      light_background: 'var(--light-background)',
      primary: 'var(--primary)',
      secondary: 'var(--secondary)',
      accent: 'var(--accent)',
      button1: 'var(--button1)',
      button2: 'var(--button2)',
      button3: 'var(--button3)',
      button4: 'var(--button4)',
    },
    borderRadius: {
      default: '10px',
    },
    extend: {},
  },
  variants: {
    extend: {},
  },
  plugins: [],
};
