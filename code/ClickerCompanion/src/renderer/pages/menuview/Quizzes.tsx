import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import React from 'react';
import { Props } from '../../types';

import MenuWrapper from '../../components/menuview/MenuWrapper';
import QuizList from '../../components/quizzes/QuizList';
import Quiz from '../../components/quizzes/Quiz';

export default function Quizzes({ setCurrentPage }: Props) {
  return (
    <MenuWrapper setCurrentPage={setCurrentPage}>
      <QuizList name="Quizzes">
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
        <Quiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />

      </QuizList>
    </MenuWrapper>
  );
}
