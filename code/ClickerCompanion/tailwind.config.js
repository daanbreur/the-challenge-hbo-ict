const colors = require('tailwindcss/colors');

module.exports = {
  content: ['./src/renderer/**/*.{js,jsx,ts,tsx,ejs}'],
  darkMode: false, // or 'media' or 'class'
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
      heading: 'Poppins',
      body: 'Poppins',
    },
    fontWeight: {
      normal: '400',
      semibold: '600',
      bold: '700',
    },
    colors: {
      text: 'var(--text)',
      background: 'var(--background)',
      primary: 'var(--primary)',
      secondary: 'var(--secondary)',
      accent: 'var(--accent)',
      button1: 'var(--button1)',
      button2: 'var(--button2)',
      button3: 'var(--button3)',
      button4: 'var(--button4)',
    },
    extend: {},
  },
  variants: {
    extend: {},
  },
  plugins: [],
};
