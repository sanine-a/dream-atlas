from random import random, choice, seed, shuffle, randint
from math import ceil
import copy

target = [ 2, 2, 3, 1, 4, 5 ]

consonants_base = [ 'p', 't', 'k', 'm', 'n' ]
vowels = [ [ 'a', 'i', 'u' ],
           [ 'a', 'i', 'u', 'e', 'o' ],
           [ 'a', 'A', 'i', 'I', 'u', 'U', 'e', 'E', 'o', 'O' ] ]

consonants_extra = [ 'b', 'd', 'j', 's', 'z', 'y', 'q', 'G', '?', 'N', 'r', 'f', 'v', 'T', 'D', 'S', 'Z', 'x', 'h', 'w', 'l', 'C' ]

sibilants = [ ['s',], [ 's', 'S' ], ['s', 'S', 'f'] ]
liquids =   [ ['r'], ['l'], ['r','l'], ['w','y'], ['r','l','w','y'] ]

orthography1 = { 'name':'nordic', 'j':'dz', 'y':'j', 'T':'th', 'D':'ð', 'S':'sh', 'Z':'zh', 'N':'ng', '?':"'", 'G':'q', 'C':'ch', 'A':'å', 'E':'ë', 'I':'ï', 'O':'ö', 'U':'ü' }
orthography2 = { 'name':'czech', 'T':'th', 'D':'th', 'S':'š', 'Z':'ž', 'C':'č', 'G':'q', 'N':'ng', '?':'-', 'A':'á', 'E':'ě', 'I':'ý', 'O':'ó', 'U':'ú' }
orthography3 = { 'name':'french', 'T':'th', 'D':'th', 'S':'ch', 'G':'gh', 'C':'tc', '?':"'", 'N':'ng', 'Z':'z', 'k':'c', 'A':'â', 'E':'ê', 'I':'î', 'O':'ô', 'U':'û' }
orthography4 = { 'name':'mexica', 'k':'c', 'G':'gh', 'N':'ng', 'T':'th', 'D':'th', 'S':'x', 'C':'ch', '?':"'", 'Z':'zh', 'A':'á', 'E':'é', 'I':'í', 'O':'ó', 'U':'ú' }

orthographies = ( orthography1, orthography2, orthography3, orthography4 )

syllables = ( [ 'CV', ],
              [ 'CV', 'V' ],
              [ 'CV', 'CVC' ],
              [ 'CV', 'CVC', 'V' ],
              [ 'CVC', ],
              [ 'CVC', 'CRVC', 'CV', 'CRV' ],
              [ 'CVC', 'CRVC', 'CVRC', 'CV', 'CRV' ],                                        [ 'CVC', 'CRVC', 'CVCC', 'CRVCC', 'CV', 'CRV' ],
              [ 'CVC', 'CRVC', 'CVRC', 'CVCC', 'CRVCC', 'CV', 'CRV' ],
              [ 'CV', 'CVC', 'SCV', 'SCVC' ],
              [ 'CVC', 'CVCC', 'SVC', 'SVCC', 'CV', 'SCV' ],
              [ 'CVC', 'CVCC', 'CRVC', 'SCVC', 'SCRVC', 'CV', 'CRV', 'SCV', 'SCRV' ] )

government = [ 'Republic of ', 'Kingdom of ', 'Confederacy of ', 'Satrapy of ','Empire of ' ]

class morpheme:
    def __init__(self,morpheme,prefix):
        self.morpheme = morpheme
        self.prefix = prefix

def elem(obj, items):
    for item in items:
        if item == obj:
            return True
    return False

def biased_choice(items, bias=2):
    i = int( random()**bias * len(items) )
    return items[i]

class language:
    def __init__(self):

        # get phonemes
        self.phonemes = {}
        self.phonemes['V'] = choice(vowels)
        shuffle(self.phonemes['V'])
        self.phonemes['R'] = choice(liquids)
        self.phonemes['S'] = choice(sibilants)
        more_consonants = []
        for i in range(0, int(random()*len(consonants_extra))):
            c = choice(consonants_extra)
            if elem(c,more_consonants):
                break
            else:
                more_consonants.append(c)
        #shuffle(more_consonants)
        self.phonemes['C'] = consonants_base + more_consonants
        shuffle(self.phonemes['C'])

        #get syllables, orthography, and word length
        self.syllables = choice(syllables)
        self.orthography = choice(orthographies)
        self.orthography[';'] = ''  # skip syllable separators
        self.wordtarget = biased_choice(target,5)

        # basic morphemes & words
        if random() >= 0.3:
            self.prefix = False
        else:
            self.prefix = True
        self.the = self.syllable()
        self.of  = self.syllable()
        self.landm = []
        for i in range(randint(3,6)):
            self.landm.append(self.shortword())
        self.waterm = []
        for i in range(randint(3,6)):
            self.waterm.append(self.shortword())
        self.citym = []
        for i in range(randint(3,6)):
            self.citym.append(self.shortword())

    def derive(self):
        derived = copy.deepcopy(self)
        if random() > 0.7:
            shuffle(derived.syllables)
        lm = 0
        wm = 0
        cm = 0
        the = False
        of = False
        if random() > 0.5:
            for i in range(randint(1,4)):
                c = choice(derived.phonemes['C'])
                if not elem(c,consonants_base):
                    derived.phonemes['C'].remove(c)
                    if elem(c,derived.the):
                        the = True
                    if elem(c,derived.of):
                        of = True
                    for m in derived.landm:
                        if elem(c,m):
                            derived.landm.remove(m)
                            lm += 1
                    for m in derived.waterm:
                        if elem(c,m):
                            derived.waterm.remove(m)
                            wm += 1
                    for m in derived.citym:
                        if elem(c,m):
                            derived.citym.remove(m)
                            cm += 1
        if random() > 0.5:
            for i in range(randint(1,4)):
                index = randint(5,len(derived.phonemes['C']))
                derived.phonemes['C'].insert(index,choice(consonants_extra))
        if the:
            derived.the = derived.syllable()
        if of:
            derived.of = derived.syllable()
        for i in range(lm):
            derived.landm.append(derived.shortword())
        for i in range(wm):
            derived.waterm.append(derived.shortword())
        for i in range(cm):
            derived.citym.append(derived.shortword())
        return derived
        
    def orthographic(self,string):
        outstring = ""
        for c in string:
            try:
                outstring += self.orthography[c]
            except KeyError:
                outstring += c
        return outstring

    def syllable(self):
        syl = ""
        stype = biased_choice(self.syllables)
        for letter in stype:
            try:
                syl = syl+biased_choice(self.phonemes[letter])
            except KeyError:
                break
        return syl+';'

    def word(self,short=False):
        w = ""
        N = randint(ceil(.5*self.wordtarget),ceil(1.5*self.wordtarget))
        if short and N >= 2:
            N -= 1
        for i in range(N):
            w = w+self.syllable()
        return w

    def shortword(self):
        sw = ""
        for i in range(randint(1,ceil(self.wordtarget))):
            sw += self.syllable()
        return sw

    def gen_name(self,morph):
        if random() < 0.1:
            return self.word() + ' ' + self.of + ' ' + self.word()
        if random() < 0.1:
            if self.prefix:
                return self.word() + ' ' + self.the
            else:
                return self.the + ' ' + self.word()
        m = ''
        if random() > 0.5:
            m = choice(morph)
        w = self.word(bool(m))
        if self.prefix:
            return m + w
        else:
            return w + m

    def cityname(self):
        return self.gen_name(self.citym)

    def landname(self):
        return self.gen_name(self.landm)

    def watername(self):
        return self.gen_name(self.waterm)

    def countryname(self):
        if random() > 0.7:
            return choice(government) + self.orthographic(self.landname()).title()
        else:
            return self.orthographic(self.landname()).title()
        
'''
lang1 = language()

for j in range(10):
    print('Language '+str(j+1))
    for i in range(5):
        word = lang1.cityname()
        print(lang1.orthographic(word).title())
    lang1 = lang1.derive()
    print(' ')
'''
