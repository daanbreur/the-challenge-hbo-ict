import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import React from 'react';
import { Props } from '../../types';

import MenuWrapper from '../../components/menuview/MenuWrapper';
import RecentList from '../../components/home/RecentList';
import RecentQuiz from '../../components/home/RecentQuiz';
import RecentReport from '../../components/home/RecentReport';

export default function Home({ setCurrentPage }: Props) {
  return (
    <MenuWrapper setCurrentPage={setCurrentPage}>
      <div className="flex-1 flex flex-row ">
        <RecentList name="Recent Quizzes">
          <RecentQuiz name="Programming 2" desc="Java arrays lorem ipsum dolor sit amet bing chilling john cena" />
          <RecentQuiz name="Databases" desc="SQL kweeries" />
          <RecentQuiz name="NOVA Pubquiz" desc="Bijeenkomst quiz" />
          <RecentQuiz name="Gang Members" desc="Lore" />
          <RecentQuiz name="SecEs Week 9" desc="Wooclap L" />
          <RecentQuiz name="Fortnite balls" desc="peepeepoopoo" />

        </RecentList>

        <RecentList name="Recent Reports">
          <RecentReport name="Programming 2" time={new Date(1704921355000)} players={10}/>
          <RecentReport name="Programming 2" time={new Date(1704921355000)} players={10} />
          <RecentReport name="Programming 2" time={new Date(1704921355000)} players={10} />
          <RecentReport name="Programming 2" time={new Date(1704921355000)} players={10} />
          <RecentReport name="Programming 2" time={new Date(1704921355000)} players={10} />
          <RecentReport name="Programming 2" time={new Date(1704921355000)} players={10} />

        </RecentList>

      </div>
    </MenuWrapper>
  );
}
