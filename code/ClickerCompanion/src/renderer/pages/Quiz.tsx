import '../globals.css';
import '../fonts.css';
import 'tailwindcss/tailwind.css';
import PageWrapper from '../components/PageWrapper';
import { Props } from '../types';

// eslint-disable-next-line @typescript-eslint/no-unused-vars
export default function Quiz({ setCurrentPage }: Props) {
  return (
    <PageWrapper>
      <footer className="text-center pt-5">
        ClickerCompanion by ClassClick B.V. 2023
      </footer>
    </PageWrapper>
  );
}
