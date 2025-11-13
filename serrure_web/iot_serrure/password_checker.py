import math
import re

# Petite liste d'exemples de mots de passe très courants (à compléter ou remplacer par une vraie base)
COMMON_PASSWORDS = {
    "123456","password","12345678","qwerty","12345","123456789","letmein","admin",
    "welcome","iloveyou","abc123","111111","123123","password1"
}

SYMBOLS = r"""!@#$%^&*()-_=+[{]}\|;:'",<.>/?`~€£¥•·"""

def estimate_entropy(password: str) -> float:
    """Estime l'entropie (en bits) selon l'alphabet utilisé * longueur."""
    pool = 0
    if re.search(r'[a-z]', password): pool += 26
    if re.search(r'[A-Z]', password): pool += 26
    if re.search(r'\d', password): pool += 10
    if re.search(r'[' + re.escape(SYMBOLS) + r']', password): pool += 32
    # si pool reste 0 (par ex. seulement espace), on considère pool=1 pour éviter log2(0)
    pool = max(pool, 1)
    return len(password) * math.log2(pool)

def has_repeated_sequence(password: str) -> bool:
    """Détecte motifs simples répétés comme 'abcabc', '121212', 'aaaa'."""
    # motif jusqu'à longueur 1/2 du mot
    n = len(password)
    for L in range(1, n//2 + 1):
        if password == password[:L] * (n // L):
            return True
    return False

def score_password(password: str) -> dict:
    """
    Retourne dict avec :
      - score (0..5)
      - entropy (bits)
      - verdict (Très faible / Faible / Moyen / Fort / Très fort)
      - warnings (liste)
      - suggestions (liste)
    """
    pwd = password or ""
    entropy = estimate_entropy(pwd)
    warnings = []
    suggestions = []

    # checks de base
    length = len(pwd)
    lower = bool(re.search(r'[a-z]', pwd))
    upper = bool(re.search(r'[A-Z]', pwd))
    digit = bool(re.search(r'\d', pwd))
    symbol = bool(re.search(r'[' + re.escape(SYMBOLS) + r']', pwd))
    space = bool(re.search(r'\s', pwd))
    common = pwd.lower() in COMMON_PASSWORDS
    repeated = has_repeated_sequence(pwd)

    # warnings
    if length == 0:
        warnings.append("Le mot de passe est vide.")
    if space:
        warnings.append("Le mot de passe contient des espaces — évite si possible.")
    if common:
        warnings.append("Mot de passe trop courant (exact match) — facile à deviner.")
    if repeated:
        warnings.append("Motif répété détecté (ex: 'abcdabcd' ou '111111').")
    if length < 8:
        warnings.append("Trop court : < 8 caractères.")
    if not (lower and upper and digit and symbol):
        # message conditionnel
        missing = []
        if not lower: missing.append("minuscules")
        if not upper: missing.append("MAJUSCULES")
        if not digit: missing.append("chiffres")
        if not symbol: missing.append("symboles")
        warnings.append("Ajoute : " + ", ".join(missing) + ".")

    # suggestions pratiques
    if length < 12:
        suggestions.append("Utilise au moins 12 caractères (ou une phrase de passe plus longue).")
    else:
        suggestions.append("Bonne longueur (>= 12).")
    if not symbol:
        suggestions.append("Ajoute des symboles (ex: !, ?, #) pour augmenter l'entropie.")
    if not (upper and lower):
        suggestions.append("Mélange majuscules et minuscules.")
    if not digit:
        suggestions.append("Ajoute des chiffres.")
    if common:
        suggestions.append("N'utilise pas un mot trop fréquent (évite '123456', 'password', ...).")
    if repeated:
        suggestions.append("Évite les motifs répétitifs ou séquences claires.")
    suggestions.append("Préfère une phrase de passe (ex: 'été!Chien45+soleil') — facile à retenir et longue.")

    # score heuristique (0..5)
    score = 0
    if length >= 8: score += 1
    if length >= 12: score += 1
    if lower and upper: score += 1
    if digit and symbol: score += 1
    if entropy >= 60 and not common and not repeated: score += 1

    # verdict
    verdict_map = {
        0: "Très faible",
        1: "Faible",
        2: "Moyen",
        3: "Fort",
        4: "Très fort",
        5: "Excellent"
    }
    verdict = verdict_map.get(score, "Inconnu")

    return {
        "password": pwd,
        "length": length,
        "entropy_bits": round(entropy, 2),
        "score": score,
        "verdict": verdict,
        "warnings": warnings,
        "suggestions": suggestions,
    }

# Petit test / usage CLI
if __name__ == "__main__":
    print("Vérificateur de mot de passe (CTRL+C pour quitter)\n")
    try:
        while True:
            p = input("Mot de passe à tester: ")
            result = score_password(p)
            print(f"\nVerdict : {result['verdict']} (score {result['score']}/5) — entropie ≈ {result['entropy_bits']} bits")
            if result['warnings']:
                print("Warnings:")
                for w in result['warnings']:
                    print("  -", w)
            if result['suggestions']:
                print("Suggestions:")
                for s in result['suggestions']:
                    print("  -", s)
            print("\n" + "-"*40 + "\n")
    except KeyboardInterrupt:
        print("\nStop.")
